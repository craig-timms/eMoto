#include "MCP23016.h"

MCP23016 PortExpand(D4, D5, 0);

void setup() {
  Serial.begin(9600);
  PortExpand.pinMode(0, DIR_INPUT);
  PortExpand.pinMode(4, DIR_INPUT);
}

void loop() {
  Serial.print(PortExpand.digitalRead(0));
  Serial.print(" - ");
  Serial.println(PortExpand.digitalRead(4));
}
