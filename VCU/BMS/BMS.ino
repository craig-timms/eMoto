#include <Arduino.h>
#include "Vehicle.h"
#include "pindef.h"
// #include "Monitor.h"
#include "CANbus.h"
// #include "Signals.h"
//#include "Motor.h"
#include "Dash.h"

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

void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  vehicle.state = 0;

  //
  CANb.setup();
  // signals.setup();
  //  motor.setup();
  // monitor.setup();
  dash.setup();
}

void loop()
{
  StartTime = millis();
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
