#include <Arduino.h>
#include "Vehicle.h"
#include "pindef.h"
// #include "Monitor.h"
#include "CANbus.h"
// #include "Signals.h"
//#include "Motor.h"
#include "Dash.h"
#include "HighVoltage.h"
#include "App.h"

unsigned long StartTime = millis();
unsigned long CurrentTime = millis();
unsigned long ElapsedTime = CurrentTime - StartTime;

unsigned long delayStart = millis();
unsigned long delay_ms = 200;

Vehicle vehicle;
// Monitor monitor;

CANbus CANb;
// Signals signals;
Dash dash;
//Motor motor;
HighVoltage HV;
//Motor motor;

void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  vehicle.state = 0;
  appSetup();

  HV.setup();

  //
  CANb.setup();
  // signals.setup();
  //  motor.setup();
  // monitor.setup();
  dash.setup();
  xTaskCreatePinnedToCore(loop1, "loop1", 4096, NULL, 1, NULL, 0);
}

void loop()
{
  StartTime = millis();
  HV.service();
  // put your main code here, to run repeatedly:
  CANb.service(  );
  // monitor.service();
  // signals.service();
  dash.service();

  //  Serial.print("Throttle: ");
  //  Serial.print(vehicle.controls.throttle);

  // Serial.print("Beams: ");
  // if (vehicle.controls.headlights)
  // {
  //   Serial.print("ON");
  // }
  // Serial.print(" - ");

  CurrentTime = millis();
  ElapsedTime = CurrentTime - StartTime;
//  Serial.print("Throttle: ");
//  Serial.print( vehicle.controls.throttle );
//  Serial.print( "    ");
//  Serial.print("Time - ");
//  Serial.println(ElapsedTime);
}

void loop1(void *pvParameters) {
  while (1) {
    appService();
    delay(1);
  }
}
