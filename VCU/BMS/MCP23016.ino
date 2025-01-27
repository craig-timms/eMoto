#include "MCP23016.h"

#include <Wire.h>

//endereço I2C do MCP23016
#define MCPAddress  0x20
 
// COMMAND BYTE TO REGISTER RELATIONSHIP : Table: 1-3 of Microchip MCP23016 - DS20090A 
//ENDEREÇOS DE REGISTRADORES
#define GP0  0x00   // DATA PORT REGISTER 0 
#define GP1  0x01   // DATA PORT REGISTER 1 
#define OLAT0  0x02   // OUTPUT LATCH REGISTER 0 
#define OLAT1  0x03   // OUTPUT LATCH REGISTER 1 
#define IPOL0   0x04  // INPUT POLARITY PORT REGISTER 0 
#define IPOL1   0x05  // INPUT POLARITY PORT REGISTER 1 
#define IODIR0  0x06  // I/O DIRECTION REGISTER 0 
#define IODIR1  0x07  // I/O DIRECTION REGISTER 1 
#define INTCAP0 0x08  // INTERRUPT CAPTURE REGISTER 0 
#define INTCAP1 0x09  // INTERRUPT CAPTURE REGISTER 1 
#define IOCON0  0x0A  // I/O EXPANDER CONTROL REGISTER 0 
#define IOCON1  0x0B  // I/O EXPANDER CONTROL REGISTER 1

void MCP23016::setup()   {
//  Serial.begin(9600);

  delay(100);
  Wire.begin();
  Wire.setClock(200000); //frequencia

  //configura o GPIO0 como OUTPUT (todos os pinos)
  configurePort(IODIR0, INPUT);
  //configura o GPIO1 como INPUT o GP1.0 e como OUTPUT os outros GP1
  configurePort(IODIR1, INPUT);
  //seta todos os pinos do GPIO0 como LOW
  writeBlockData(GP0, B00000000);
  //seta todos os pinos do GPIO1 como LOW
  writeBlockData(GP1, B00000000);
}

void MCP23016::readAll() {
  //verifica e o botão GP foi pressionado
    
  Serial.print(readPin(7, GP1));
//  Serial.print(" - ");
  Serial.print(readPin(6, GP1));
//  Serial.print(" - ");
  Serial.print(readPin(5, GP1));
//  Serial.print(" - ");
  Serial.print(readPin(4, GP1));
//  Serial.print(" - ");
  Serial.print(readPin(3, GP1));
//  Serial.print(" - ");
  Serial.print(readPin(2, GP1));
//  Serial.print(" - ");
  Serial.print(readPin(1, GP1));
//  Serial.print(" - ");
  Serial.print(readPin(0, GP1));
  
  Serial.print(readPin(7, GP0));
//  Serial.print(" - ");
  Serial.print(readPin(6, GP0));
//  Serial.print(" - ");
  Serial.print(readPin(5, GP0));
//  Serial.print(" - ");
  Serial.print(readPin(4, GP0));
//  Serial.print(" - ");
  Serial.print(readPin(3, GP0));
//  Serial.print(" - ");
  Serial.print(readPin(2, GP0));
//  Serial.print(" - ");
  Serial.print(readPin(1, GP0));
//  Serial.print(" - ");
  Serial.println(readPin(0, GP0));
//  Serial.print(" --- ");

}

int MCP23016::readArr()
{
  uint8_t arr[16] = {0};
  uint8_t statusGP0 = 0;
  uint8_t statusGP1 = 0;
  int out = 0;

  Wire.setClock(200000); //frequencia

  Wire.beginTransmission(MCPAddress);
  Wire.write(GP0);
  Wire.endTransmission();
  Wire.requestFrom(MCPAddress, 1); // ler do chip  1 byte
  statusGP0 = Wire.read();
//  Serial.print(statusGP0, BIN);

  Wire.beginTransmission(MCPAddress);
  Wire.write(GP1);
  Wire.endTransmission();
  Wire.requestFrom(MCPAddress, 1); // ler do chip  1 byte
  statusGP1 = Wire.read();
//  Serial.print(" - ");
//  Serial.println(statusGP1, BIN);

  out = ( (int)statusGP1 << 8) | statusGP0;

  return out;
  
}

//configura o GPIO (GP0 ou GP1)
//como parametro passamos:
//port: GP0 ou GP1
//custom: INPUT para todos as portas do GP trabalharem como entrada
//        OUTPUT para todos as portas do GP trabalharem como saida
//        custom um valor de 0-255 indicando o modo das portas (1=INPUT, 0=OUTPUT)
//        ex: 0x01 ou B00000001 ou  1 : indica que apenas o GPX.0 trabalhará como entrada, o restando como saida
void MCP23016::configurePort(uint8_t port, uint8_t custom)
{
  if (custom == INPUT)
  {
    writeBlockData(port, 0xFF);
  }
  else if (custom == OUTPUT)
  {
    writeBlockData(port, 0x00);
  }
  else
  {
    writeBlockData(port, custom);
  }
}

//envia dados para o MCP23016 através do barramento i2c
//cmd: COMANDO (registrador)
//data: dados (0-255)
void MCP23016::writeBlockData(uint8_t cmd, uint8_t data)
{
  Wire.beginTransmission(MCPAddress);
  Wire.write(cmd);
  Wire.write(data);
  Wire.endTransmission();
  delay(10);
}

//verifica se o botão foi pressionado
//parametro GP: GP0 ou GP1
void MCP23016::checkButton(uint8_t GP)
{
  //faz a leitura do pino 0 no GP fornecido
  uint8_t btn = readPin(0, GP);
  //se botão pressionado, seta para HIGH as portas GP0
  if (btn)
  {
    writeBlockData(GP0, B11111111);
  }
  //caso contrario deixa todas em estado LOW
  else {
    writeBlockData(GP0, B00000000);
  }
}

//faz a leitura de um pino específico
//pin: pino desejado (0-7)
//gp: GP0 ou GP1
//retorno: 0 ou 1
uint8_t MCP23016::readPin(uint8_t pin, uint8_t gp)
{
  uint8_t statusGP = 0;
  Wire.beginTransmission(MCPAddress);
  Wire.write(gp);
  Wire.endTransmission();
  Wire.requestFrom(MCPAddress, 1); // ler do chip  1 byte
  statusGP = Wire.read();

  return valueFromPin(pin, statusGP);
}

//retorna o valor do bit na posição desejada
//pin: posição do bit (0-7)
//statusGP: valor lido do GP (0-255)
uint8_t MCP23016::valueFromPin(uint8_t pin, uint8_t statusGP)
{
  return (statusGP & ( 0x0001 << pin)) == 0 ? 0 : 1;
}
