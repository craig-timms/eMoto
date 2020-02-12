#include <Arduino.h>
#include "Motor.h"
#include "ESP32_BLE.h"

Motor motor;

unsigned long delayStart = 0;
unsigned long delay_ms = 200;

void setup() 
{
  //debug serial ouput
  Serial.begin( 115200 );

  //module setup
  motor.setup();

  delay(500);
  BLE_setup();
  delayStart = millis();

  delay(500); // pause for stuff to stabilize
}

void loop() {

  motor.service();
//  delay(10);
  if ( millis() > delayStart + delay_ms ) {
    
//    Serial.print("Time since BT send: ");
//    Serial.print(millis()-delayStart);
//    Serial.println();
    
    BLE_update( motor.getRPM(), motor.getCurrent(), motor.getVoltage(), motor.getTempInv() );
    delayStart = millis();
  }
  
}
