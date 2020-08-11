#include "MCP23016.h"

unsigned long StartTime = micros();
unsigned long CurrentTime = micros();
unsigned long ElapsedTime = CurrentTime  - StartTime;

MCP23016 gpioExpander;
int out = 0;

void setup()   {
  gpioExpander.setup();
}

void loop() {
  //  StartTime = micros();
//  gpioExpander.readAll();
  //  CurrentTime = micros();
  //  ElapsedTime = CurrentTime - StartTime;
  //  Serial.println(ElapsedTime);


  StartTime = micros();
  out = gpioExpander.readArr();

  for (int i = 15; i >= 0; i--) {
    Serial.print( bitRead(out, i) );
  }
  
  CurrentTime = micros();
  ElapsedTime = CurrentTime - StartTime;
  Serial.print( " - " );
  Serial.println( ElapsedTime );

}
