#include "CANbus.h"

void CANbus::setup(void)
{
  Serial.println("CAN Setup -- Started");

  // CAN_cfg.speed = CAN_SPEED_250KBPS;
  // CAN_cfg.tx_pin_id = GPIO_NUM_17; // Standby mode GPIO_NUM_5
  // CAN_cfg.rx_pin_id = GPIO_NUM_16; // GPIO_NUM_4
  // CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  // // Init CAN Module
  // ESP32Can.CANInit();

  // start the CAN bus at 250 kbps
  if (!CAN.begin(250E3))
  {
    Serial.println("Starting CAN failed!");
    while (1)
      ;
  }

  Serial.println("CAN Setup -- Finished");
}

void CANbus::restart(void)
{
}

void CANbus::service(void)
{
  readBus();

  // Sending
  if ((millis() - tMsg) > tMsgPeriod)
  {
    sendControls();
    tMsg = millis();
  }
}

void CANbus::sendCharger(bool charge, int voltage, int current, char LED)
{
  uint8_t c_B0 = 0x05;
  if (charge)
  {
    c_B0 = 0x04;
  }

  uint8_t c_B2 = vehicle.charger.vMax / 256;
  uint8_t c_B1 = vehicle.charger.vMax - (c_B2 * 256);

  uint8_t c_B4 = (vehicle.charger.iMax + 32000) / 256;
  uint8_t c_B3 = (vehicle.charger.iMax + 32000) - (c_B4 * 256);

  uint8_t c_B5 = 0x01;
  if (LED == 'r')
  {
    c_B5 = 0x00;
  }
  else if (LED == 'y')
  {
    c_B5 = 0x02;
  }
  else if (LED == 'Y')
  {
    c_B5 = 0x03;
  }
  else if (LED == 'g')
  {
    c_B5 = 0x04;
  }
  else if (LED == 'G')
  {
    c_B5 = 0x05;
  }

  // CAN_frame_t tx_frame;
  // tx_frame.FIR.B.FF = CAN_frame_ext;
  // tx_frame.MsgID = 0x18E54024;
  // tx_frame.FIR.B.DLC = 8;
  // tx_frame.data.u8[0] = c_B0; // 0x05-stop  0x04-start
  // tx_frame.data.u8[1] = c_B1; // voltage
  // tx_frame.data.u8[2] = c_B2; //
  // tx_frame.data.u8[3] = c_B3; // current
  // tx_frame.data.u8[4] = c_B4; //
  // tx_frame.data.u8[5] = c_B5; // LED
  // tx_frame.data.u8[6] = 0x06; // 0xFF
  // tx_frame.data.u8[7] = 0x07; // 0xFF
  // ESP32Can.CANWriteFrame(&tx_frame);
  // Serial.println("Charger CAN sent");
}

void CANbus::readBus(void)
{
  // int packetSize = CAN.parsePacket();
  uint8_t msg[8] = {0};
  int packets = 0;

  while (CAN.parsePacket() > 0)
  {
    packets++;
    for (int msgi = 0; msgi < 8; msgi++)
    {
      if (CAN.available())
      {
        msg[msgi] = CAN.read();
      }
    }

    int ID = CAN.packetId();

    if (ID == CANID_MCU_1)
    {
      getMCU1(msg);
      timerMCU = millis();
    }
    else if (ID == CANID_MCU_2)
    {
      getMCU2(msg);
    }
    else if (ID == CANID_CHARGER_SET)
    {
      getCharger(msg);
      timerOBC = millis();
    }

    if (millis() < (timerMCU + period))
    {
      vehicle.mcu.online = true;
    }
    else
    {
      vehicle.mcu.online = false;
    }

    if (millis() < (timerBMS + period))
    {
      vehicle.battery.online = true;
    }
    else
    {
      vehicle.battery.online = false;
    }

    if (millis() < (timerOBC + period))
    {
      vehicle.charger.online = true;
    }
    else
    {
      vehicle.charger.online = false;
    }
  }
}

void CANbus::sendControls(void)
{
  //  Serial.println();
  //  Serial.print("Sending packet ... ");
  // Bool
  uint8_t bit0 = 0;

  bitWrite(bit0, 0, vehicle.controls.key);
  bitWrite(bit0, 1, vehicle.controls.brake);
  bitWrite(bit0, 2, vehicle.controls.horn);
  bitWrite(bit0, 3, vehicle.controls.headlights);
  bitWrite(bit0, 4, vehicle.controls.S1);
  bitWrite(bit0, 5, vehicle.controls.S2);
  bitWrite(bit0, 6, vehicle.controls.AUX1);
  bitWrite(bit0, 7, vehicle.controls.AUX2);
  bitWrite(bit0, 8, vehicle.controls.AUX3);
  bitWrite(bit0, 9, vehicle.controls.AUX4);

  //  Serial.print("bools set - ");
  //  Serial.print(bit0, HEX);
  //  Serial.print(" - ");

  // Throttle
  uint8_t bit1 = vehicle.controls.throttle / 5;
  uint8_t bit2 = vehicle.controls.regen / 5;

  // Other
  uint8_t bit3 = vehicle.controls.gear;
  uint8_t bit4 = vehicle.controls.intensity;
  uint8_t bit5 = vehicle.controls.turn;
  uint8_t bit6 = vehicle.controls.angel;
  uint8_t bit7 = vehicle.controls.state;

  //  Serial.print("ints set - ");
  //  Serial.print(bit1, HEX);
  //  Serial.print(" ");
  //  Serial.print(bit2, HEX);
  //  Serial.print(" - ");

  CAN.beginExtendedPacket(CANID_CONTROLS);
  CAN.write(bit0);
  CAN.write(bit1);
  CAN.write(bit2);
  CAN.write(bit3);
  CAN.write(bit4);
  CAN.write(bit5);
  CAN.write(bit6);
  CAN.write(bit7);
  CAN.endPacket();
  //  Serial.println("Controls Sent");
  delay(2);
  //  Serial.println("Controls CAN sent");
}

void CANbus::sendOnline(void)
{
}

bool CANbus::getOnline(void)
{
  bool out = false;

  return out;
}

void CANbus::getMCU1(uint8_t msgIn[])
{
  timerMCU = millis();
  // Speed, Current, Voltage
  vehicle.mcu.RPM = ((msgIn[1] * 256) + msgIn[0]);
  vehicle.mcu.current = ((msgIn[3] * 256) + msgIn[2]) / 10;
  vehicle.mcu.voltage = (((int)msgIn[5] * 256) + (int)msgIn[4]) / 10;

  vehicle.mcu.errorsA = msgIn[6];
  vehicle.mcu.errorsB = msgIn[7];
}

void CANbus::getMCU2(uint8_t msgIn[])
{
  vehicle.mcu.rThrottle = msgIn[0];
  vehicle.mcu.temp = msgIn[1] - 40;
  vehicle.mcu.tempM = msgIn[2] - 30;

  vehicle.mcu.rDirection = (bitRead(msgIn[4], 3) << 1) | bitRead(msgIn[4], 2);
  vehicle.mcu.rCmd = (bitRead(msgIn[4], 1) << 1) | bitRead(msgIn[4], 0);
  vehicle.mcu.rSwStatus = msgIn[5];
}

void CANbus::getCharger(uint8_t msgIn[])
{
}

void CANbus::getBattery(uint8_t msgIn[])
{
  // TODO
}
