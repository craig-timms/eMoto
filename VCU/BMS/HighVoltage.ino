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
//  if ( vehicle.battery.appPre ) {
//    setPrecharge( true );
//  } else
//  {
//    setPrecharge( false );
//  }
//  
//  if ( vehicle.battery.appBleed ) {
//    setBleed( true );
//  } else
//  {
//    setBleed( false );
//  }
  
  if ( (vehicle.battery.appHV) && (HVstatus==0) ) {
    enable( true );
  } else if ( (!vehicle.battery.appHV) && ((HVstatus==1)||(HVstatus==2)) )
  {
    enable( false );
  }


  if ( (HVstatus == 1 ) && (millis() > HVcntrlTimer + HVonHoldoff) ) {  
    setContactor( true );  
    setPrecharge( false );
    Serial.println("HV on");
    HVstatus = 2;
  } else if ( (HVstatus == 3 ) && (millis() > HVcntrlTimer + HVoffHoldoff) ) {
    setBleed( false );
    HVstatus = 0;
    Serial.println("HV off");
  } else if ( (HVstatus == 3 ) && (millis() > HVcntrlTimer + offDelayContactor) ) {
    setBleed( true );
  }

  if ( HVstatus == 0 ) {            // OFF
    // TODO
    vehicle.battery.HV = false;
    vehicle.battery.discharge = false;
    vehicle.battery.precharge = false;
  } else if ( HVstatus == 1 ) {     // ON
    // TODO
    vehicle.battery.HV = true;
    vehicle.battery.discharge = false;
    vehicle.battery.precharge = true;
  } else if ( HVstatus == 2 ) {     // ON
    // TODO
    vehicle.battery.HV = true;
    vehicle.battery.discharge = false;
    vehicle.battery.precharge = false;
  } else if ( HVstatus == 3 ) {     // ON
    // TODO
    vehicle.battery.HV = true;
    vehicle.battery.discharge = true;
    vehicle.battery.precharge = false;
  } 
  
}

void HighVoltage::enable( bool en )
{
    if ( en ) { // turn-on
//      Serial.println("Confirmed -- turn-on");
      setBleed( false );
      setContactor( false );
      setPrecharge( true );
      HVstatus = 1;
      HVcntrlTimer = millis();
    } else { // turn-off
      // TODO do some pre-shutdown checks
      setBleed( false );
      setContactor( false );
      setPrecharge( false );
      HVstatus = 3;
      HVcntrlTimer = millis();   
    }
}

void HighVoltage::restart(void)
{
  // TODO
}

void HighVoltage::getBatteryInfo( void )
{
  int16_t results;
 
  results = ads1015.readADC_Differential_0_1();
  
  // Serial.print("Battery: "); 
  // Serial.print(results); 
  // Serial.print("("); 
  // Serial.print(results * GAIN_VISO * 10); 
  // Serial.println("mV)");

  vehicle.battery.voltage = results * GAIN_VISO;
  // TODO
  // vehicle.battery.current = ;
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
