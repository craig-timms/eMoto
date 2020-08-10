#include "Motor.h"

void
Motor::setup( void ) 
{

  pinMode(GPIO_MCU_POWER, OUTPUT);
  setMCUpower( true );

  pinMode(GPIO_BRAKE_AN, OUTPUT);
  setBrakeAN( false );

  pinMode(GPIO_FOOT, OUTPUT);
  setFoot( false );

  delay(400);
}

void
Motor::restart( void )
{
    // TODO
}

void
Motor::setMCUpower( bool setTo )
{
    if (setTo) {
      digitalWrite(GPIO_MCU_POWER, HIGH);
    } else
    {
      digitalWrite(GPIO_MCU_POWER, LOW);
    }
    
}

void
Motor::setBrakeAN( bool setTo )
{
    if (setTo) {
      digitalWrite(GPIO_BRAKE_AN, HIGH);
    } else
    {
      digitalWrite(GPIO_BRAKE_AN, LOW);
    }
    
}

void
Motor::setFoot( bool setTo )
{
    if (setTo) {
      digitalWrite(GPIO_FOOT, HIGH);
    } else
    {
      digitalWrite(GPIO_FOOT, LOW);
    }
    
}

void
Motor::service( void )
{
    // TODO
    // if (not working) restart;

    // TODO
    // if (error) writeThrottle(0);

    writeThrottle();
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
Motor::setThrottle( int throttle )
{
    // ledcWrite(0, throttleOUT);
    dacWrite(DAC_THROTTLE_OUT, throttle);
//    Serial.println("Throttle SET");
}

void
Motor::setBrake( int setTo )
{
    // ledcWrite(0, throttleOUT);
    dacWrite(DAC_BRAKE_OUT, setTo);
//    Serial.println("Throttle SET");
}
