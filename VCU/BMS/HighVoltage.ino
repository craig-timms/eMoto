#include "HighVoltage.h"

void HighVoltage::setup(void)
{
  ads1015.begin();  // Initialize ads1015
  ads1015.setGain(GAIN_TWO);
  
  pinMode(GPIO_CONTACTOR_1, OUTPUT);
  // pinMode(GPIO_CONTACTOR_2, OUTPUT);
  // pinMode(GPIO_PRE, OUTPUT);
  pinMode(GPIO_PRE_2, OUTPUT);
  pinMode(GPIO_BLEED, OUTPUT);
  // pinMode(GPIO_BLEED_2, OUTPUT);
  // pinMode(GPIO_BLEED_T, OUTPUT);
  // pinMode(GPIO_BLEED_B, OUTPUT);

  setContactor(false);
  // setContactor2(false);
  setPrecharge(false);
  // setPrecharge2(false);
  setBleed(false);
  // setBleed2(false);
  // setBleedT(false);
  // setBleedB(false);

  delay(100);
}

void HighVoltage::service(void)
{
  // TODO
  // if (not working) restart;

  // TODO
  // if (error) writeThrottle(0);
  if ( vehicle.battery.appPre ) {
    setPrecharge( true );
  } else
  {
    setPrecharge( false );
  }
  
  if ( vehicle.battery.appBleed ) {
    setBleed( true );
  } else
  {
    setBleed( false );
  }

  int16_t results;
 
  results = ads1015.readADC_Differential_0_1();
//  Serial.print("Differential: "); 
//  Serial.print(results); 
//  Serial.print("("); 
//  Serial.print(results * 2); 
//  Serial.println("mV)");
  
  Serial.print("Battery: "); 
  Serial.print(results); 
  Serial.print("("); 
  Serial.print(results * GAIN_VISO * 10); 
  Serial.println("mV)");

  vehicle.battery.voltage = results * GAIN_VISO;
  
}

void HighVoltage::restart(void)
{
  // TODO
}

void HighVoltage::setContactor(bool setTo)
{
  if (setTo)
  {
    digitalWrite(GPIO_CONTACTOR_1, HIGH);
  }
  else
  {
    digitalWrite(GPIO_CONTACTOR_1, LOW);
  }
}

void HighVoltage::setPrecharge(bool setTo)
{
  if (setTo)
  {
    digitalWrite(GPIO_PRE_2, HIGH);
  }
  else
  {
    digitalWrite(GPIO_PRE_2, LOW);
  }
}

void HighVoltage::setBleed(bool setTo)
{
  if (setTo)
  {
    digitalWrite(GPIO_BLEED, HIGH);
  }
  else
  {
    digitalWrite(GPIO_BLEED, LOW);
  }
}
