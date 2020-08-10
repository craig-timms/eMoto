#include <Arduino.h>
#include "Vehicle.h"
#include "Monitor.h"
#include "CANbus.h"
#include "Signals.h"
#include "Motor.h"

Vehicle vehicle;
Monitor monitor;

CANbus CANb;
Signals signals;
Motor motor;
void setup() {
  // put your setup code here, to run once:
  vehicle.state = 0;

  // 
  CANb.setup();
  signals.setup();
  motor.setup();

  monitor.setup();


}

void loop() {
  // put your main code here, to run repeatedly:

}
