#include <Arduino.h>
#include "pindef.h"
#include "Vehicle.h"
#include "Monitor.h"
#include "CANbus.h"
#include "Signals.h"
#include "Motor.h"
#include "Dash.h"
#include "App.h"

unsigned long StartTime = millis();
unsigned long CurrentTime = millis();
unsigned long ElapsedTime = CurrentTime - StartTime;

Vehicle vehicle;
Monitor monitor;

CANbus CANb;
Signals signals;
Dash dash;
Motor motor;

unsigned long delayStart = 0;
unsigned long delay_ms = 200;
int L = 1;

void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  vehicle.state = 0;
  delay(200);

  appSetup();
  Serial.println("App setup");
  CANb.setup();
  Serial.println("CAN setup");
  signals.setup();
  Serial.println("Signals setup");
  monitor.setup();
  Serial.println("Monitor setup");
  dash.setup();
  Serial.println("Dash setup");
  motor.setup();
  Serial.println("Motor setup");

  delayStart = millis();

  xTaskCreatePinnedToCore(loop1, "loop1", 4096, NULL, 1, NULL, 0);
}

void loop()
{
  StartTime = millis();

  if ( vehicle.controls.state==0 && vehicle.controls.gear==0 ) {
    vehicle.controls.state = 1;
  }


  //  Serial.print("MONITOR");
  monitor.service();
  //  Serial.print(" - ");

  //  Serial.print("SIGNALS");
  signals.service();
  //  Serial.print(" - ");

  dash.service();
  //  Serial.print("DASH");
  //  Serial.print(" - ");

  //  Serial.print("Throttle: ");
  //  Serial.println(vehicle.controls.throttle);

  //  Serial.print("Beams: ");
  //  if (vehicle.controls.headlights)
  //  {
  //    Serial.print("ON");
  //  }
  //  Serial.print(" - ");
  //

  CANb.service();
  motor.service();

  //  CurrentTime = millis();
  //  ElapsedTime = CurrentTime - StartTime;
  //  Serial.print(" - ");
  //  Serial.println(ElapsedTime);
}

void loop1(void *pvParameters) {
  while (1) {
    appService();
    delay(1);
  }
}
