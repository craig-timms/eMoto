#include <Arduino.h>
#include <ESP32CAN.h>
#include <CAN_config.h>

CAN_device_t CAN_cfg;               // CAN Config
unsigned long previousMillis = 0;   // will store last time a CAN Message was send
const int interval = 1000;          // interval at which send CAN Messages (milliseconds)
const int rx_queue_size = 10;       // Receive Queue size
int CANrx = 16;
int CANtx = 17;

// throttle
const int GPIO_throttleIN = 39;
const int GPIO_keyIN = 36;
const int GPIO_throttleOUT = 23;
int throttleOUT = 0;
int tINf[6] = {0, 0, 0, 0, 0, 0};
int tINi = 0;
int throttleIN = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Basic Demo - ESP32-Arduino-CAN");
  CAN_cfg.speed = CAN_SPEED_250KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_17;
  CAN_cfg.rx_pin_id = GPIO_NUM_16;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  // Init CAN Module
  ESP32Can.CANInit();

  // throttle
  ledcSetup(0, 10000, 12); // (dunno, frequency, bits)
  ledcAttachPin(GPIO_throttleOUT,0);
  
  Serial.println("SETUP");
}

int throttleRead = 0;
int tempInv = 0;
int tempMtr = 0;
char statusInv[ 16 + 1 ];
char statusCmd = 'N'; 
char statusFb = 'N'; 
int RPM = 0;
int currentMtr = 0;
int voltageDC = 0;
char ERR[ 16 + 1 ];

void loop() {
  
  CAN_frame_t rx_frame;

  unsigned long currentMillis = millis();

  // throttle
  throttleIN = (analogRead(GPIO_throttleIN)-630) * 1000 / (4095-630);
  if (throttleIN<0) { throttleIN = 0; }
//  tINf[tINi] = analogRead(GPIO_throttleIN);
//  throttleIN = (tINf[0] + tINf[1] + tINf[2] + tINf[3] + tINf[4] + tINf[5]) / 6;
//  if (tINi<6) { 
//    tINi++; 
//  } else { 
//    tINi=0; 
//  } 
  throttleOUT = (throttleIN*4095)/1000;
  ledcWrite(0, throttleOUT);

  // Receive next CAN frame from queue
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {

//    if (rx_frame.FIR.B.FF == CAN_frame_std) {
//      printf("New standard frame");
//    }
//    else {
//      printf("New extended frame");
//    }

//    if (rx_frame.FIR.B.RTR == CAN_RTR) {
//      printf(" RTR from 0x%08X, DLC %d\r\n", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
//    }
//    else {
//      printf(" from 0x%08X, DLC %d, Data ", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
//      for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
//        printf("0x%02X ", rx_frame.data.u8[i]);
//      }
//      printf("\n");
//    }

    // Message 1
    if ( rx_frame.MsgID == 0x0CF11E05 ) {

      // Bits
      int bit0 = rx_frame.data.u8[0];
      int bit1 = rx_frame.data.u8[1];
      int bit2 = rx_frame.data.u8[2];
      int bit3 = rx_frame.data.u8[3];
      int bit4 = rx_frame.data.u8[4];
      int bit5 = rx_frame.data.u8[5];
      uint8_t bit6 = rx_frame.data.u8[6];
      uint8_t bit7 = rx_frame.data.u8[7];

      // Speed, Current, Voltage
      RPM = ( (bit1*256) + bit0 );
      currentMtr = ( (bit3*256) + bit2 ) / 10;
      voltageDC = ( (bit5*256) + bit4 ) / 10;

      uint8_t bitsCount = 8;
//      char ERR[ bitsCount*2 + 1 ];
      
      uint8_t i = 0;
//      uint8_t j = 0;
      while ( i < bitsCount ) {
        ERR[ i ] = bitRead( bit6, i ) + '0';
        i+=1;
      } 
      while ( (i < bitsCount*2) && (i >= bitsCount) ) {
        ERR[ i ] = bitRead( bit7, i - bitsCount ) + '0';
        i+=1;
      }
      ERR[ i ] = '\0';
      
//      printf(" RTR from 0x%08X ", rx_frame.MsgID);
//      Serial.print("RPM: ");
//      Serial.print(RPM);
//      Serial.print("  I: ");
//      Serial.print(currentMtr);
//      Serial.print("  VDC: ");
//      Serial.print(voltageDC);
//      Serial.print("  ERRORS: ");
//      Serial.print(ERR);
//      Serial.println();
    }

    // Message 2
    if ( rx_frame.MsgID == 0x0CF11F05 ) {

      // Bits
      int bit0 = rx_frame.data.u8[0];
      int bit1 = rx_frame.data.u8[1];
      int bit2 = rx_frame.data.u8[2];
      int bit3 = rx_frame.data.u8[3];
      uint8_t bit4 = rx_frame.data.u8[4];
      uint8_t bit5 = rx_frame.data.u8[5];
      uint8_t bit6 = rx_frame.data.u8[6];
      uint8_t bit7 = rx_frame.data.u8[7];

      // Speed, Current, Voltage
      float temp = ( (float)bit0 * (100.0/255.0) );
      throttleRead = (int)temp;
      tempInv = bit1 - 40;
      tempMtr = bit2 - 30;

      uint8_t bitsCount = 8;
//      char statusInv[ bitsCount*2 + 1 ];
      
      
      uint8_t i = 0;
//      uint8_t j = 0;
      while ( i < bitsCount ) {
        statusInv[ i ] = bitRead( bit4, i ) + '0';
        i+=1;
      } 
      while ( (i < bitsCount*2) && (i >= bitsCount) ) {
        statusInv[ i ] = bitRead( bit5, i - bitsCount ) + '0';
        i+=1;
      }
      statusInv[ i ] = '\0';

      statusCmd = 'N';     
      if ( (statusInv[0] == '0') & (statusInv[1] == '0') ) {
        statusCmd = 'N';
      } else if ( (statusInv[0] == '1') & (statusInv[1] == '0') ) {
        statusCmd = 'F';
      } else if ( (statusInv[0] == '0') & (statusInv[1] == '1') ) {
        statusCmd = 'R';
      }

      statusFb = 'N';     
      if ( (statusInv[2] == '0') & (statusInv[3] == '0') ) {
        statusFb = 'N';
      } else if ( (statusInv[2] == '1') & (statusInv[3] == '0') ) {
        statusFb = 'F';
      } else if ( (statusInv[2] == '0') & (statusInv[3] == '1') ) {
        statusFb = 'R';
      }

    }

    if ( (rx_frame.MsgID == 0x0CF11E05) || (rx_frame.MsgID == 0x0CF11E05) ) {
      Serial.print("RPM: ");
      Serial.print(RPM);
      Serial.print("  I: ");
      Serial.print(currentMtr);
      Serial.print("  VDC: ");
      Serial.print(voltageDC);
      Serial.print("  ERRORS: ");
      Serial.print(ERR);
      Serial.print("  ThIN: ");
      Serial.print(throttleIN);
      Serial.print("  ThR: ");
      Serial.print(throttleRead);
      Serial.print("  Ti: ");
      Serial.print(tempInv);
      Serial.print("  Tm: ");
      Serial.print(tempMtr);
      Serial.print("  CMD: ");
      Serial.print(statusCmd);
      Serial.print("  FB: ");
      Serial.print(statusFb);
      Serial.println();
    }
    
  }
  // Send CAN Message
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    CAN_frame_t tx_frame;
    tx_frame.FIR.B.FF = CAN_frame_std;
    tx_frame.MsgID = 0x001;
    tx_frame.FIR.B.DLC = 8;
    tx_frame.data.u8[0] = 0x00;
    tx_frame.data.u8[1] = 0x01;
    tx_frame.data.u8[2] = 0x02;
    tx_frame.data.u8[3] = 0x03;
    tx_frame.data.u8[4] = 0x04;
    tx_frame.data.u8[5] = 0x05;
    tx_frame.data.u8[6] = 0x06;
    tx_frame.data.u8[7] = 0x07;
    ESP32Can.CANWriteFrame(&tx_frame);
    Serial.println("SENT");
  }
}
