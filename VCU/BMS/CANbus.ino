#include "CANbus.h"

void CANbus::setup(void)
{
  Serial.println("CAN Setup -- Started");

  // start the CAN bus at 250 kbps
  if (!CAN.begin(250E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
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
    // TODO
    tMsg = millis();
    if ( vehicle.charger.enable ) {
      sendCharger( true, vehicle.charger.vMax, vehicle.charger.iMax, 'G' );
      Serial.println("Charger enable CAN sent");
    } else {
      sendCharger( false, vehicle.charger.vMax, vehicle.charger.iMax, 'G' );
      //      Serial.println("Charger DISABLE CAN sent");
    }
  }

  vehicle.charger.online = false;
  if ( millis() < (timerOBC + period) ) {
    vehicle.charger.online = true;
  }
  vehicle.mcu.online = false;
  if ( millis() < (timerMCU + period) ) {
    vehicle.mcu.online = true;
  }
}

void CANbus::sendCharger(bool charge, int voltage, int current, char LED)
{
  uint8_t bit0 = 0xFD;
  if (charge)
  {
    bit0 = 0xFC;
  }

  // uint8_t bit2 = vehicle.charger.vMax*10 / 256;
  // uint8_t bit1 = vehicle.charger.vMax*10 - (bit2 * 256);
  //  uint8_t bit1 = 0xA0;
  //  uint8_t bit2 = 0x02;
  uint8_t bit1 = vehicle.charger.vMax * 10 - 512;
  uint8_t bit2 = 0x02;


  // uint8_t bit4 = (vehicle.charger.iMax + 32000) / 256;
  // uint8_t bit3 = (vehicle.charger.iMax + 32000) - (bit4 * 256);
  uint8_t bit4 = 0x0C;
  uint8_t bit3 = 128;
  if (vehicle.charger.iMax <= 0) {
    // do nothing
  } else if ( vehicle.charger.iMax > 255 ) {
    bit3 = 255;
  } else {
    bit3 = 10 * vehicle.charger.iMax + 128;
  }

  uint8_t bit5 = 0x01;
  if (LED == 'r')
  {
    bit5 = 0x00;
  }
  else if (LED == 'y')
  {
    bit5 = 0x02;
  }
  else if (LED == 'Y')
  {
    bit5 = 0x03;
  }
  else if (LED == 'g')
  {
    bit5 = 0x04;
  }
  else if (LED == 'G')
  {
    bit5 = 0x05;
  }

  CAN.beginExtendedPacket(CANID_CHARGER_SET);
  CAN.write(bit0);
  CAN.write(bit1);
  CAN.write(bit2);
  CAN.write(bit3);
  CAN.write(bit4);
  CAN.write(bit5);
  CAN.write(0xFF);
  CAN.write(0xFF);
  CAN.endPacket();
  // Serial.print("5 - ");
  delay(2);
  //  Serial.println("Charger CAN sent");
}

void CANbus::readBus(void)
{
//  int packetSize = CAN.parsePacket();
  uint8_t msg[8] = { 0 };
  int packets = 0;

  while ( CAN.parsePacket() != 0 )
  {
    //    Serial.print("Packet Size: ");
    //    Serial.println(packetSize);
    packets++;
    for (int msgi = 0; msgi < 8; msgi++)
    {
      if (CAN.available())
      {
        msg[msgi] = CAN.read();
      }
    }

    while( CAN.available() ) {
      CAN.read();
    }

    if (CAN.packetId() == CANID_CONTROLS)
    {
      getControls( msg );
    }
    else if (CAN.packetId() == CANID_MCU_1)
    {
      getMCU1( msg );
    }
    else if (CAN.packetId() == CANID_MCU_2)
    {
      getMCU2( msg );
    }
    else if (CAN.packetId() == CANID_CHARGER_GET)
    {
      getCharger( msg );
    }
  }

  if ( packets > 0 ) {
//    Serial.print("Packets: ");
//    Serial.println(packets);
  }


}

void CANbus::getControls( uint8_t msgIn[] )
{
  // Bool
  vehicle.controls.key = bitRead(msgIn[0], 0);
  vehicle.controls.brake = bitRead(msgIn[0], 1);
  vehicle.controls.horn = bitRead(msgIn[0], 2);
  vehicle.controls.headlights = bitRead(msgIn[0], 3);
  vehicle.controls.S1 = bitRead(msgIn[0], 4);
  vehicle.controls.S2 = bitRead(msgIn[0], 5);
  vehicle.controls.AUX1 = bitRead(msgIn[0], 6);
  vehicle.controls.AUX2 = bitRead(msgIn[0], 7);
  vehicle.controls.AUX3 = bitRead(msgIn[0], 8);
  vehicle.controls.AUX4 = bitRead(msgIn[0], 9);

  // Throttle
  vehicle.controls.throttle = 5 * msgIn[1];
  vehicle.controls.regen = 5 * msgIn[2];

  // Other
  vehicle.controls.gear = msgIn[3];
  vehicle.controls.intensity = msgIn[4];
  vehicle.controls.turn = msgIn[5];
  vehicle.controls.angel = msgIn[6];
}

void CANbus::sendControls(void)
{
  Serial.println();
  Serial.print("Sending packet ... ");
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

  Serial.print("bools set - ");
  Serial.print(bit0, HEX);
  Serial.print(" - ");

  // Throttle
  uint8_t bit1 = vehicle.controls.throttle / 5;
  uint8_t bit2 = vehicle.controls.regen / 5;

  // Other
  uint8_t bit3 = vehicle.controls.gear;
  uint8_t bit4 = vehicle.controls.intensity;
  uint8_t bit5 = vehicle.controls.turn;
  uint8_t bit6 = vehicle.controls.angel;

  Serial.print("ints set - ");
  Serial.print(bit1, HEX);
  Serial.print(" ");
  Serial.print(bit2, HEX);
  Serial.print(" - ");

  CAN.beginExtendedPacket(CANID_CONTROLS);
  CAN.write(bit0);
  CAN.write(bit1);
  CAN.write(bit2);
  CAN.write(bit3);
  CAN.write(bit4);
  CAN.write(bit5);
  CAN.write(bit6);
  CAN.write(0x00);
  CAN.endPacket();
  // Serial.print("5 - ");
  delay(2);
  Serial.println("Controls CAN sent");
}

void CANbus::sendOnline(void)
{
  // CAN_frame_t tx_frame;
  // tx_frame.FIR.B.FF = CAN_frame_ext;
  // tx_frame.MsgID = 0x100;
  // tx_frame.FIR.B.DLC = 8;
  // // tx_frame.data.u8[0] = bit0; // 0x05-stop  0x04-start
  // // tx_frame.data.u8[1] = bit1; // voltage
  // // tx_frame.data.u8[2] = bit2; //
  // // tx_frame.data.u8[3] = bit3; // current
  // // tx_frame.data.u8[4] = bit4; //
  // // tx_frame.data.u8[5] = bit6; // LED
  // tx_frame.data.u8[0] = 0x01; // 0x05-stop  0x04-start
  // tx_frame.data.u8[1] = 0x00; // voltage
  // tx_frame.data.u8[2] = 0x00; //
  // tx_frame.data.u8[3] = 0x00; // current
  // tx_frame.data.u8[4] = 0x00; //
  // tx_frame.data.u8[5] = 0x00; // LED
  // tx_frame.data.u8[6] = 0x00; // 0xFF
  // tx_frame.data.u8[7] = 0x00; // 0xFF
  // ESP32Can.CANWriteFrame(&tx_frame);
  // Serial.println("Online sent");
}

bool CANbus::getOnline(void)
{
  bool out = false;
  //   CAN_frame_t rx_frame;

  //   // Receive next CAN frame from queue
  //   if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE)
  //   {
  //     Serial.print("   Something");

  //     // Motor Message 1
  //     if (rx_frame.MsgID == 0x100)
  //     {

  //       // Bits
  //       uint8_t bit0 = rx_frame.data.u8[0];
  //       uint8_t bit1 = rx_frame.data.u8[1];
  //       uint8_t bit2 = rx_frame.data.u8[2];
  //       uint8_t bit3 = rx_frame.data.u8[3];
  //       uint8_t bit4 = rx_frame.data.u8[4];
  //       uint8_t bit5 = rx_frame.data.u8[5];
  //       uint8_t bit6 = rx_frame.data.u8[6];
  //       uint8_t bit7 = rx_frame.data.u8[7];

  //       if ( bit1 == 0x01 ) { out = true; }

  //       Serial.print( bit0, HEX );
  //       Serial.print(" ");
  //       Serial.print( bit1, HEX );
  //       Serial.print(" ");
  //       Serial.print( bit2, HEX );
  //       Serial.println();

  //       // Bool
  // //      vehicle.controls.key = bitRead(bit0, 0);
  // //      vehicle.controls.brake = bitRead(bit0, 1);
  // //      vehicle.controls.horn = bitRead(bit0, 2);
  // //      vehicle.controls.headlights = bitRead(bit0, 3);
  // //      vehicle.controls.S1 = bitRead(bit0, 4);
  // //      vehicle.controls.S2 = bitRead(bit0, 5);
  // //      vehicle.controls.AUX1 = bitRead(bit0, 6);
  // //      vehicle.controls.AUX2 = bitRead(bit0, 7);
  // //      vehicle.controls.AUX3 = bitRead(bit0, 8);
  // //      vehicle.controls.AUX4 = bitRead(bit0, 9);
  //     }
  //   }

  return out;
}


void CANbus::getMCU2( uint8_t msgIn[] )
{
  // TODO
}

void CANbus::getBattery( uint8_t msgIn[] )
{
  // TODO
}


// if (rx_frame.MsgID == 0x0CF11E05)
//   // Bits
//   int bit0 = rx_frame.data.u8[0];
// int bit1 = rx_frame.data.u8[1];
// int bit2 = rx_frame.data.u8[2];
// int bit3 = rx_frame.data.u8[3];
// int bit4 = rx_frame.data.u8[4];
// int bit5 = rx_frame.data.u8[5];
// uint8_t bit6 = rx_frame.data.u8[6];
// uint8_t bit7 = rx_frame.data.u8[7];

// // Speed, Current, Voltage
// mRPM = ((bit1 * 256) + bit0);
// mCurrent = ((bit3 * 256) + bit2) / 10;
// mVoltage = ((bit5 * 256) + bit4) / 10;

// uint8_t bitsCount = 8;
// //      char ERR[ bitsCount*2 + 1 ];

// uint8_t i = 0;
// //      uint8_t j = 0;
// while (i < bitsCount)
// {
//   mERR[i] = bitRead(bit6, i) + '0';
//   i += 1;
// }
// while ((i < bitsCount * 2) && (i >= bitsCount))
// {
//   mERR[i] = bitRead(bit7, i - bitsCount) + '0';
//   i += 1;
// }
// mERR[i] = '\0';
// }

// // Motor Message 2
// if (rx_frame.MsgID == 0x0CF11F05)
// {

//   // Bits
//   int bit0 = rx_frame.data.u8[0];
//   int bit1 = rx_frame.data.u8[1];
//   int bit2 = rx_frame.data.u8[2];
//   int bit3 = rx_frame.data.u8[3];
//   uint8_t bit4 = rx_frame.data.u8[4];
//   uint8_t bit5 = rx_frame.data.u8[5];
//   uint8_t bit6 = rx_frame.data.u8[6];
//   uint8_t bit7 = rx_frame.data.u8[7];

//   // Speed, Current, Voltage
//   float temp = ((float)bit0 * (100.0 / 255.0));
//   mThrottle = (int)temp;
//   mTempI = bit1 - 40;
//   mTemp = bit2 - 30;

//   uint8_t statusInv[16 + 1];
//   uint8_t bitsCount = 8;
//   //      char statusInv[ bitsCount*2 + 1 ];
//   uint8_t i = 0;
//   while (i < bitsCount)
//   {
//     statusInv[i] = bitRead(bit4, i) + '0';
//     i += 1;
//   }
//   while ((i < bitsCount * 2) && (i >= bitsCount))
//   {
//     statusInv[i] = bitRead(bit5, i - bitsCount) + '0';
//     i += 1;
//   }
//   statusInv[i] = '\0';

//   mGearCmd = 'N';
//   if ((statusInv[0] == '0') & (statusInv[1] == '0'))
//   {
//     mGearCmd = 'N';
//   }
//   else if ((statusInv[0] == '1') & (statusInv[1] == '0'))
//   {
//     mGearCmd = 'F';
//   }
//   else if ((statusInv[0] == '0') & (statusInv[1] == '1'))
//   {
//     mGearCmd = 'R';
//   }

//   mGearFb = 'N';
//   if ((statusInv[2] == '0') & (statusInv[3] == '0'))
//   {
//     mGearFb = 'N';
//   }
//   else if ((statusInv[2] == '1') & (statusInv[3] == '0'))
//   {
//     mGearFb = 'F';
//   }
//   else if ((statusInv[2] == '0') & (statusInv[3] == '1'))
//   {
//     mGearFb = 'R';
//   }
// }

void CANbus::getMCU1(uint8_t msgIn[])
{
  timerMCU = millis();
  // Speed, Current, Voltage
  vehicle.mcu.RPM = ((msgIn[1] * 256) + msgIn[0]);
  vehicle.mcu.current = ((msgIn[3] * 256) + msgIn[2]) / 10;
  vehicle.mcu.voltage = ((msgIn[5] * 256) + msgIn[4]) / 10;
  vehicle.mcu.errorA = msgIn[6];
  vehicle.mcu.errorb = msgIn[7];

  uint8_t bitsCount = 8;
  //      char ERR[ bitsCount*2 + 1 ];

  uint8_t i = 0;
  //      uint8_t j = 0;
  while (i < bitsCount)
  {
    mERR[i] = bitRead(msgIn[6], i) + '0';
    i += 1;
  }
  while ((i < bitsCount * 2) && (i >= bitsCount))
  {
    mERR[i] = bitRead(msgIn[7], i - bitsCount) + '0';
    i += 1;
  }
  mERR[i] = '\0';
}


void CANbus::getCharger( uint8_t msgIn[] )
{
  timerOBC = millis();
  vehicle.charger.online = true;

  vehicle.charger.rV = (msgIn[3] * 256) + msgIn[2];
  vehicle.charger.rI = ((msgIn[5] * 256) + msgIn[4]) - 3200;


  vehicle.charger.rOn = false;
  if ((bitRead(msgIn[1], 0) == '1') & (bitRead(msgIn[1], 1) == '0'))
  {
    vehicle.charger.rOn = true;
  }

  vehicle.charger.eTemp = true;
  if ( (bitRead(msgIn[0], 6) == '1') & (bitRead(msgIn[0], 7) == '0'))
  {
    vehicle.charger.eTemp = false;
  }

  vehicle.charger.eVac = true;
  if ( (bitRead(msgIn[0], 4) == '1') & (bitRead(msgIn[0], 5) == '0'))
  {
    vehicle.charger.eVac = false;
  }

  vehicle.charger.eHW = true;
  if ((bitRead(msgIn[0], 2) == '1') & (bitRead(msgIn[0], 3) == '0'))
  {
    vehicle.charger.eHW = false;
  }

  vehicle.charger.eCom = true;
  if ((bitRead(msgIn[0], 0) == '1') & (bitRead(msgIn[0], 1) == '0'))
  {
    vehicle.charger.eCom = false;
  }
}
