#include "CANbus.h"

void CANbus::setup(void)
{
  Serial.println();
  Serial.println("CAN Setup -- Started");
  CAN_cfg.speed = CAN_SPEED_250KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_17;
  CAN_cfg.rx_pin_id = GPIO_NUM_16;
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
  // Init CAN Module
  ESP32Can.CANInit();

  delay(100);
  Serial.print("sending... ");
//  sendOnline();
  Serial.println("sent ");
  delay(100);
  canStartTime = millis();

  Serial.println("CAN Setup -- Finished");
}

void CANbus::restart(void)
{

  Serial.println("CAN Restarting");
  // TODO
  ESP32Can.CANStop();
  delay(100);
  
  ESP32Can.CANInit();

  delay(100);
  Serial.print("sending... ");
//  sendOnline();
  Serial.println("sent ");
  delay(100);
  canStartTime = millis();

  Serial.println("CAN Restarted");
}

void CANbus::service(void)
{
  getControls();
  if ( getOnline() ) {
    sendOnline();
    canStartTime = millis();
  }
  
  canCurrentTime = millis();
  canElapsedTime =  canCurrentTime - canStartTime;

  if ( canElapsedTime > 8000 ) {
    restart();
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

  CAN_frame_t tx_frame;
  tx_frame.FIR.B.FF = CAN_frame_ext;
  tx_frame.MsgID = 0x18E54024;
  tx_frame.FIR.B.DLC = 8;
  tx_frame.data.u8[0] = c_B0; // 0x05-stop  0x04-start
  tx_frame.data.u8[1] = c_B1; // voltage
  tx_frame.data.u8[2] = c_B2; //
  tx_frame.data.u8[3] = c_B3; // current
  tx_frame.data.u8[4] = c_B4; //
  tx_frame.data.u8[5] = c_B5; // LED
  tx_frame.data.u8[6] = 0x06; // 0xFF
  tx_frame.data.u8[7] = 0x07; // 0xFF
  ESP32Can.CANWriteFrame(&tx_frame);
  Serial.println("Charger CAN sent");
}

void CANbus::read(void)
{

  CAN_frame_t rx_frame;

  unsigned long currentMillis = millis();

  // Receive next CAN frame from queue
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE)
  {

    // Motor Message 1
    if (rx_frame.MsgID == 0x0CF11E05)
    {

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
      mRPM = ((bit1 * 256) + bit0);
      mCurrent = ((bit3 * 256) + bit2) / 10;
      mVoltage = ((bit5 * 256) + bit4) / 10;

      uint8_t bitsCount = 8;
      //      char ERR[ bitsCount*2 + 1 ];

      uint8_t i = 0;
      //      uint8_t j = 0;
      while (i < bitsCount)
      {
        mERR[i] = bitRead(bit6, i) + '0';
        i += 1;
      }
      while ((i < bitsCount * 2) && (i >= bitsCount))
      {
        mERR[i] = bitRead(bit7, i - bitsCount) + '0';
        i += 1;
      }
      mERR[i] = '\0';
    }

    // Motor Message 2
    if (rx_frame.MsgID == 0x0CF11F05)
    {

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
      float temp = ((float)bit0 * (100.0 / 255.0));
      mThrottle = (int)temp;
      mTempI = bit1 - 40;
      mTemp = bit2 - 30;

      uint8_t statusInv[16 + 1];
      uint8_t bitsCount = 8;
      //      char statusInv[ bitsCount*2 + 1 ];
      uint8_t i = 0;
      while (i < bitsCount)
      {
        statusInv[i] = bitRead(bit4, i) + '0';
        i += 1;
      }
      while ((i < bitsCount * 2) && (i >= bitsCount))
      {
        statusInv[i] = bitRead(bit5, i - bitsCount) + '0';
        i += 1;
      }
      statusInv[i] = '\0';

      mGearCmd = 'N';
      if ((statusInv[0] == '0') & (statusInv[1] == '0'))
      {
        mGearCmd = 'N';
      }
      else if ((statusInv[0] == '1') & (statusInv[1] == '0'))
      {
        mGearCmd = 'F';
      }
      else if ((statusInv[0] == '0') & (statusInv[1] == '1'))
      {
        mGearCmd = 'R';
      }

      mGearFb = 'N';
      if ((statusInv[2] == '0') & (statusInv[3] == '0'))
      {
        mGearFb = 'N';
      }
      else if ((statusInv[2] == '1') & (statusInv[3] == '0'))
      {
        mGearFb = 'F';
      }
      else if ((statusInv[2] == '0') & (statusInv[3] == '1'))
      {
        mGearFb = 'R';
      }
    }

    // Charger Message In
    if (rx_frame.MsgID == 0x18EB2440)
    { // Charger status

      // Bits
      int bit0 = rx_frame.data.u8[0];
      int bit1 = rx_frame.data.u8[1];
      int bit2 = rx_frame.data.u8[2];
      int bit3 = rx_frame.data.u8[3];
      int bit4 = rx_frame.data.u8[4];
      int bit5 = rx_frame.data.u8[5];
      uint8_t bit6 = rx_frame.data.u8[6];
      uint8_t bit7 = rx_frame.data.u8[7];

      cVoltage = (bit3 * 256) + bit2;
      cCurrent = ((bit5 * 256) + bit4) - 3200;

      //
      char ERR[8 + 1] = {};
      char ERR2[8 + 1] = {};

      uint8_t bitsCount = 8; //      char ERR[ bitsCount*2 + 1 ];
      uint8_t i = 0;
      while (i < bitsCount)
      {
        ERR[i] = bitRead(bit0, i) + '0';
        ERR2[i] = bitRead(bit1, i) + '0';
        i += 1;
      }

      ERR[i] = '\0';
      ERR2[i] = '\0';

      cCharging = false;
      if ((ERR2[0] == '1') & (ERR2[1] == '0'))
      {
        cCharging = true;
      }

      cErrorTemp = true;
      if ((ERR[6] == '1') & (ERR[7] == '0'))
      {
        cErrorTemp = false;
      }

      cErrorVac = true;
      if ((ERR[4] == '1') & (ERR[5] == '0'))
      {
        cErrorVac = false;
      }

      cErrorHW = true;
      if ((ERR[2] == '1') & (ERR[3] == '0'))
      {
        cErrorHW = false;
      }

      cErrorCom = true;
      if ((ERR[0] == '1') & (ERR[1] == '0'))
      {
        cErrorCom = false;
      }

      Serial.print(ERR2[0]);
      Serial.print(ERR2[1]);
      Serial.print(ERR2[2]);
      Serial.print(ERR2[3]);
      Serial.print(ERR2[4]);
      Serial.print(ERR2[5]);
      Serial.print(ERR2[6]);
      Serial.print(ERR2[7]);
      Serial.println();
      // Serial.println(bit2, DEC);
      // Serial.println(bit3, DEC);
    }

    if (((rx_frame.MsgID == 0x0CF11E05) || (rx_frame.MsgID == 0x0CF11E05)))
    {
      //      Serial.print("RPM: ");
      //      Serial.print(mRPM);
      //      Serial.print("  I: ");
      //      Serial.print(mCurrent);
      //      Serial.print("  VDC: ");
      //      Serial.print(mVoltage);
      //      Serial.print("  ERRORS: ");
      //      Serial.print(mERR);
      //      Serial.print("  ThIN: ");
      //      Serial.print(throttleIN);
      //      Serial.print("  ThR: ");
      //      Serial.print(throttleRead);
      //      Serial.print("  Ti: ");
      //      Serial.print(tempInv);
      //      Serial.print("  Tm: ");
      //      Serial.print(tempMtr);
      //      Serial.print("  CMD: ");
      //      Serial.print(mGearCmd);
      //      Serial.print("  FB: ");
      //      Serial.print(mGearFb);
      //      Serial.println();
    }
  }
}

void CANbus::getControls(void)
{

  CAN_frame_t rx_frame;

  // Receive next CAN frame from queue
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE)
  {

    // Motor Message 1
    if (rx_frame.MsgID == CANID_CONTROLS)
    {

      // Bits
      uint8_t bit0 = rx_frame.data.u8[0];
      uint8_t bit1 = rx_frame.data.u8[1];
      uint8_t bit2 = rx_frame.data.u8[2];
      uint8_t bit3 = rx_frame.data.u8[3];
      uint8_t bit4 = rx_frame.data.u8[4];
      uint8_t bit5 = rx_frame.data.u8[5];
      uint8_t bit6 = rx_frame.data.u8[6];
      uint8_t bit7 = rx_frame.data.u8[7];

      // Bool
      vehicle.controls.key = bitRead(bit0, 0);
      vehicle.controls.brake = bitRead(bit0, 1);
      vehicle.controls.horn = bitRead(bit0, 2);
      vehicle.controls.headlights = bitRead(bit0, 3);
      vehicle.controls.S1 = bitRead(bit0, 4);
      vehicle.controls.S2 = bitRead(bit0, 5);
      vehicle.controls.AUX1 = bitRead(bit0, 6);
      vehicle.controls.AUX2 = bitRead(bit0, 7);
      vehicle.controls.AUX3 = bitRead(bit0, 8);
      vehicle.controls.AUX4 = bitRead(bit0, 9);

      // Throttle
      vehicle.controls.throttle = 5 * bit1;
      vehicle.controls.regen = 5 * bit2;
      Serial.println();
      Serial.print("Throttle: ");
      Serial.println(bit1);

      // Other
      vehicle.controls.gear = bit3;
      vehicle.controls.intensity = bit4;
      vehicle.controls.turn = bit5;
      vehicle.controls.angel = bit6;
    }
  }
}

void CANbus::sendControls( void )
{
  // Bool
  uint8_t bit0 = 0;

  bitWrite( bit0, 0, vehicle.controls.key );
  bitWrite(bit0, 1, vehicle.controls.brake );
  bitWrite(bit0, 2, vehicle.controls.horn );
  bitWrite(bit0, 3, vehicle.controls.headlights );
  bitWrite(bit0, 4, vehicle.controls.S1 );
  bitWrite(bit0, 5, vehicle.controls.S2 );
  bitWrite(bit0, 6, vehicle.controls.AUX1 );
  bitWrite(bit0, 7, vehicle.controls.AUX2 );
  bitWrite(bit0, 8, vehicle.controls.AUX3 );
  bitWrite(bit0, 9, vehicle.controls.AUX4 );

  // Throttle
  uint8_t bit1 = vehicle.controls.throttle / 5;
  uint8_t bit2 = vehicle.controls.regen / 5;

  // Other
  uint8_t bit3 = vehicle.controls.gear;
  uint8_t bit4 = vehicle.controls.intensity;
  uint8_t bit5 = vehicle.controls.turn;
  uint8_t bit6 = vehicle.controls.angel;

  CAN_frame_t tx_frame;
  tx_frame.FIR.B.FF = CAN_frame_ext;
  tx_frame.MsgID = CANID_CONTROLS;
  tx_frame.FIR.B.DLC = 8;
  // tx_frame.data.u8[0] = bit0; // 0x05-stop  0x04-start
  // tx_frame.data.u8[1] = bit1; // voltage
  // tx_frame.data.u8[2] = bit2; //
  // tx_frame.data.u8[3] = bit3; // current
  // tx_frame.data.u8[4] = bit4; //
  // tx_frame.data.u8[5] = bit6; // LED
  tx_frame.data.u8[0] = 0x01; // 0x05-stop  0x04-start
  tx_frame.data.u8[1] = 0x01; // voltage
  tx_frame.data.u8[2] = 0x01; //
  tx_frame.data.u8[3] = 0x01; // current
  tx_frame.data.u8[4] = 0x01; //
  tx_frame.data.u8[5] = 0x01; // LED
  tx_frame.data.u8[6] = 0x01; // 0xFF
  tx_frame.data.u8[7] = 0x01; // 0xFF
  ESP32Can.CANWriteFrame(&tx_frame);
  Serial.println("Charger CAN sent");
}

void CANbus::sendOnline( void )
{
  CAN_frame_t tx_frame;
  tx_frame.FIR.B.FF = CAN_frame_ext;
  tx_frame.MsgID = 0x100;
  tx_frame.FIR.B.DLC = 8;
  // tx_frame.data.u8[0] = bit0; // 0x05-stop  0x04-start
  // tx_frame.data.u8[1] = bit1; // voltage
  // tx_frame.data.u8[2] = bit2; //
  // tx_frame.data.u8[3] = bit3; // current
  // tx_frame.data.u8[4] = bit4; //
  // tx_frame.data.u8[5] = bit6; // LED
  tx_frame.data.u8[0] = 0x00; // 0x05-stop  0x04-start
  tx_frame.data.u8[1] = 0x01; // voltage
  tx_frame.data.u8[2] = 0x00; //
  tx_frame.data.u8[3] = 0x00; // current
  tx_frame.data.u8[4] = 0x00; //
  tx_frame.data.u8[5] = 0x00; // LED
  tx_frame.data.u8[6] = 0x00; // 0xFF
  tx_frame.data.u8[7] = 0x00; // 0xFF
  ESP32Can.CANWriteFrame(&tx_frame);
  Serial.println("Online sent");
  
}

bool CANbus::getOnline(void)
{
  Serial.print("   Serviced");
  bool out = false;
  CAN_frame_t rx_frame;

  // Receive next CAN frame from queue
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE)
  {

    // Motor Message 1
    if (rx_frame.MsgID == 0x100)
    {
      Serial.print("-0x100");

      // Bits
      uint8_t bit0 = rx_frame.data.u8[0];
      uint8_t bit1 = rx_frame.data.u8[1];
      uint8_t bit2 = rx_frame.data.u8[2];
      uint8_t bit3 = rx_frame.data.u8[3];
      uint8_t bit4 = rx_frame.data.u8[4];
      uint8_t bit5 = rx_frame.data.u8[5];
      uint8_t bit6 = rx_frame.data.u8[6];
      uint8_t bit7 = rx_frame.data.u8[7];

      if ( bit0 == 0x01 ) { out = true; }

      // Bool
//      vehicle.controls.key = bitRead(bit0, 0);
//      vehicle.controls.brake = bitRead(bit0, 1);
//      vehicle.controls.horn = bitRead(bit0, 2);
//      vehicle.controls.headlights = bitRead(bit0, 3);
//      vehicle.controls.S1 = bitRead(bit0, 4);
//      vehicle.controls.S2 = bitRead(bit0, 5);
//      vehicle.controls.AUX1 = bitRead(bit0, 6);
//      vehicle.controls.AUX2 = bitRead(bit0, 7);
//      vehicle.controls.AUX3 = bitRead(bit0, 8);
//      vehicle.controls.AUX4 = bitRead(bit0, 9);
    }
  }

  return out;
}
