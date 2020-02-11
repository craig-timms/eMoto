#include <Arduino.h>
#include "Motor.h"

Motor motor;

void setup() 
{
  //debug serial ouput
  Serial.begin( 115200 );

  //module setup
  motor.setup();

  delay(100); // pause for stuff to stabilize
}

void loop() {

  motor.service();

}
