#include "Motor.h"

void
Motor::setup( void ) 
{
  // throttle pin config
  ledcSetup(0, 10000, 12);              // Config PWM output (dunno, frequency, bits)
  ledcAttachPin(GPIO_throttleOUT, 0);   // Config throttle out pin

  // HV control config
  pinMode(GPIO_precharge, OUTPUT);
  pinMode(GPIO_contactor, OUTPUT);
  pinMode(GPIO_discharge, OUTPUT);
  digitalWrite(GPIO_precharge, LOW);
  digitalWrite(GPIO_contactor, LOW);
  digitalWrite(GPIO_discharge, LOW);
  HVcntrlTimer = millis();

  delay(400);
}

void
Motor::setHV( int cmdHV )
{
    HVstatus = cmdHV;
    if ( cmdHV == 1 ) { // turn-on
//      Serial.println("Confirmed -- turn-on");
      digitalWrite(GPIO_discharge, LOW);
      digitalWrite(GPIO_contactor, LOW);
      digitalWrite(GPIO_precharge, HIGH);
      HVcntrlTimer = millis();
      delay(200);
    } else if ( cmdHV == 3 ) { // turn-off
//      Serial.println("Confirmed -- turn-off");
      digitalWrite(GPIO_discharge, LOW);
      digitalWrite(GPIO_precharge, LOW);
      digitalWrite(GPIO_contactor, LOW);
      HVcntrlTimer = millis();
      delay(200);
      digitalWrite(GPIO_discharge, HIGH);      
    }
}

void
Motor::HVcontrol( void )
{
  if ( HVstatus == 0 ) {            // OFF
    // TODO
  } else if ( HVstatus == 2 ) {     // ON
    // TODO
  } else if ( (HVstatus == 1 ) && (millis() > HVcntrlTimer + HVonHoldoff) ) {    
    digitalWrite(GPIO_contactor, HIGH);
    digitalWrite(GPIO_precharge, LOW);
    Serial.println("HV on");
    HVstatus = 2;
    delay(100);
//  } else if ( (HVstatus == 3 ) && (millis() > HVcntrlTimer + 1000) ) {
//    digitalWrite(GPIO_discharge, HIGH);
//    Serial.println("Bleedin");
//    delay(10);
  } else if ( (HVstatus == 3 ) && (millis() > HVcntrlTimer + HVoffHoldoff) ) {
    digitalWrite(GPIO_discharge, LOW);
    HVstatus = 0;
    Serial.println("HV off");
    delay(100);
  }

}

void
Motor::restart( void )
{
    // TODO
}

void
Motor::service( void )
{
    // TODO
    // if (not working) restart;

    // TODO
    // if (error) writeThrottle(0);

    readThrottle();
    writeThrottle();
    HVcontrol();
}

int
Motor::readThrottle( void )
{
  throttleIN = (analogRead(GPIO_throttleIN)-630) * 1000 / (4095-630);
  if (throttleIN<0) { throttleIN = 0; }
  throttleOUT = (throttleIN*4095)/1000;
//  Serial.println("Throttle GOT");

  return throttleOUT;
}

void
Motor::writeThrottle( void )
{
    ledcWrite(0, throttleOUT);
//    Serial.println("Throttle SET");
}
