#include <Arduino.h>
#include "Motor.h"
#include "ESP32_BLE.h"

Motor motor;

void setup() 
{
  //debug serial ouput
  Serial.begin( 115200 );

  //module setup
  motor.setup();

  BLE_setup();

  delay(100); // pause for stuff to stabilize
}

void loop() {

  motor.service();
  BLE_update( motor.getRPM(), motor.getCurrent(), motor.getVoltage(), motor.getTempInv() )

}
