// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>

uint8_t bit0;
uint8_t bit1;
uint8_t bit2;
uint8_t bit3;
uint8_t bit4;
uint8_t bit5;
uint8_t bit6;
uint8_t bit7;

uint8_t msg[8] = {0};

unsigned long period = 0;
unsigned long timer = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Receiver");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(250E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  Serial.println("Starting CAN good!");
}

void loop() {
  // try to parse packet
  timer = millis();
  int packetSize = CAN.parsePacket();

  if (packetSize) {
    //    Serial.println(millis() - timer);
    // received a packet
    //    Serial.print("Packet size ");
    //    Serial.println(packetSize);
    //    Serial.print("Received ");

    //    if (CAN.packetExtended()) {
    //      Serial.print("extended ");
    //    }
    //
    //    if (CAN.packetRtr()) {
    //      // Remote transmission request, packet contains no data
    //      Serial.print("RTR ");
    //    }

    Serial.print("id:  0x");
    Serial.println(CAN.packetId(), HEX);

    //    if (CAN.packetRtr()) {
    //      Serial.print(" and requested length ");
    //      Serial.println(CAN.packetDlc());
    //    } else {
    //      Serial.print(" and length ");
    //      Serial.println(packetSize);

    // only print packet data for non-RTR packets
    //      while (CAN.available()) {
    //        Serial.print(CAN.read()); // (char)
    //      }

    if (CAN.available()) {
      msg[0] = CAN.read();
    }
    if (CAN.available()) {
      msg[1] = CAN.read();
    }
    if (CAN.available()) {
      msg[2] = CAN.read();
    }
    if (CAN.available()) {
      msg[3] = CAN.read();
    }
    if (CAN.available()) {
      msg[4] = CAN.read();
    }
    if (CAN.available()) {
      msg[5] = CAN.read();
    }
    if (CAN.available()) {
      msg[6] = CAN.read();
    }
    if (CAN.available()) {
      msg[7] = CAN.read();
    }

    interpControls( msg );

//    Serial.print("Msg: ");
//    Serial.print(" 0x");
//    Serial.print(bit0, HEX);
//    Serial.print(" 0x");
//    Serial.print(bit1, HEX);
//    Serial.print(" 0x");
//    Serial.print(bit2, HEX);
//    Serial.print(" 0x");
//    Serial.print(bit3, HEX);
//    Serial.print(" 0x");
//    Serial.print(bit4, HEX);
//    Serial.print(" 0x");
//    Serial.print(bit5, HEX);
//    Serial.print(" 0x");
//    Serial.print(bit6, HEX);
//    Serial.print(" 0x");
//    Serial.print(bit7, HEX);

    Serial.println();
    Serial.print("Time: ");
    Serial.println(millis() - timer);

    Serial.println();

    //    }
    //    packetSize = CAN.parsePacket();
  }


  //  Serial.println("............");

}

void interpControls( uint8_t msgIn[] ) {
  Serial.print("Msg from function: ");
  Serial.print(" 0x");
  Serial.print(msgIn[0], HEX);
  Serial.print(" 0x");
  Serial.print(msgIn[1], HEX);
  Serial.print(" 0x");
  Serial.print(msgIn[2], HEX);
  Serial.print(" 0x");
  Serial.print(msgIn[3], HEX);
  Serial.print(" 0x");
  Serial.print(msgIn[4], HEX);
  Serial.print(" 0x");
  Serial.print(msgIn[5], HEX);
  Serial.print(" 0x");
  Serial.print(msgIn[6], HEX);
  Serial.print(" 0x");
  Serial.print(msgIn[7], HEX);
  Serial.println();
}
