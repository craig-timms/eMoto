#include <Arduino.h>
#include "Motor.h"
#include "CANbus.h"
#include "Charger.h"
#include "Lights.h"

Motor motor;
Charger charger;
CANbus CANb;
Lights lights;

#include "ESP32_BLE.h"


unsigned long delayStart = 0;
unsigned long delay_ms = 200;
bool print_mtr_CAN = false;
bool print_chg_CAN = false;

void setup() 
{
  //debug serial ouput
  Serial.begin( 9600 );

  //module setup
  motor.setup();
  CANb.setup();
  lights.setup();

  delay(200);
  BLE_setup();
  delayStart = millis();

  delay(200); // pause for stuff to stabilize
}

void loop() {

  motor.service();
  CANb.read();
  lights.service();
  
  // Bluetooth update every 200 ms
  if ( millis() > delayStart + delay_ms ) {    
    BLE_update( );
    delayStart = millis();
    if ( charger.cmdOn ) {
      CANb.sendCharger(true, charger.vMax*10, charger.iMax*10, 'R');
    }
  }
  
}
