// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>

uint8_t bit0 = 0x01;
uint8_t bit1 = 0x00;
uint8_t bit2 = 0x01;
uint8_t bit3 = 0x00;
uint8_t bit4 = 0x01;
uint8_t bit5 = 0x00;
uint8_t bit6 = 0x01;
uint8_t bit7 = 0x01;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Sender");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(250E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
  Serial.print("Sending packet ... ");

  CAN.beginPacket(0x12);
  CAN.write('h');
  CAN.write('e');
  CAN.write('l');
  CAN.write('l');
  CAN.write('o');
  CAN.endPacket();

  Serial.println("done");

  delay(1000);

  // send extended packet: id is 29 bits, packet can contain up to 8 bytes of data
  Serial.print("Sending extended packet ... ");

  CAN.beginExtendedPacket(0xabcdef);
  CAN.write(bit0);
  CAN.write(bit1);
  CAN.write(bit2);
  CAN.write(bit3);
  CAN.write(bit4);
  CAN.write(bit5);
  CAN.write(bit6);
  CAN.write(bit7);
  CAN.endPacket();
  delay(2);

  CAN.beginExtendedPacket(0x10);
  CAN.write(0x09);
  CAN.write(0x09);
  CAN.write(0x09);
  CAN.write(0x09);
  CAN.write(0x09);
  CAN.write(0x09);
  CAN.write(0x09);
  CAN.write(0x09);
  CAN.endPacket();
  delay(2);

  Serial.println("done");

  delay(1000);
}
