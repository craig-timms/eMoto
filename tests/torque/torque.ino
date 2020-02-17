#include <Arduino.h>
#include "Motor.h"
Motor motor;

#include "ESP32_BLE.h"


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
  
  // Bluetooth update every 200 ms
  if ( millis() > delayStart + delay_ms ) {    
    BLE_update( motor.getRPM(), motor.getCurrent(), motor.getVoltage(), motor.getTempInv() );
    delayStart = millis();
  }
  
}
