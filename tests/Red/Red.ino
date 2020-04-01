#include "FastLED.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// RGB Calibration code
//
// Use this sketch to determine what the RGB ordering for your chipset should be.  Steps for setting up to use:

// * Uncomment the line in setup that corresponds to the LED chipset that you are using.  (Note that they
//   all explicitly specify the RGB order as RGB)
// * Define DATA_PIN to the pin that data is connected to.
// * (Optional) if using software SPI for chipsets that are SPI based, define CLOCK_PIN to the clock pin
// * Compile/upload/run the sketch

// You should see six leds on.  If the RGB ordering is correct, you should see 1 red led, 2 green
// leds, and 3 blue leds.  If you see different colors, the count of each color tells you what the
// position for that color in the rgb orering should be.  So, for example, if you see 1 Blue, and 2
// Red, and 3 Green leds then the rgb ordering should be BRG (Blue, Red, Green).

// You can then test this ordering by setting the RGB ordering in the addLeds line below to the new ordering
// and it should come out correctly, 1 red, 2 green, and 3 blue.
//
//////////////////////////////////////////////////

#define NUM_LEDS_B 12
#define NUM_TURN_B 4

#define NUM_LEDS_turn 4

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.

// good 22, 13, 1, 2, 5
// not good 6, 7, 10, 11

#define DATA_PIN_B 5  // front
#define DATA_PIN_L 1  // left
#define DATA_PIN_R 3  // right



boolean blinker = false;
unsigned long blinkerTimer = 0;
unsigned long blinkerPeriod = 400;

CRGB leds_B[NUM_LEDS_B];
CRGB leds_L[NUM_LEDS_turn];
CRGB leds_R[NUM_LEDS_turn];

void setup() {
    // sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(1000);

    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
     FastLED.addLeds<WS2811, DATA_PIN_B, GRB>(leds_B, NUM_LEDS_B);
     FastLED.addLeds<WS2811, DATA_PIN_L, GRB>(leds_L, NUM_LEDS_turn);
     FastLED.addLeds<WS2811, DATA_PIN_R, GRB>(leds_R, NUM_LEDS_turn);

    // FastLED.setBrightness(CRGB(255,255,255));
//    FastLED.setBrightness( CRGB(80,80,80) );
    FastLED.setBrightness( 100 );
    
    blinkerTimer = millis();
}

void loop() {
  FastLED.setBrightness( 30 );

  // Bluetooth update every 200 ms
  if ( millis() < blinkerTimer + blinkerPeriod ) {    
    blinker = true;
  } else if ( millis() < blinkerTimer + 2*blinkerPeriod ) {
    blinker = false;
  } else {
    blinkerTimer = millis();
  }
  

  for(int i = 0; i < NUM_LEDS_turn; i = i + 1) {
    if ( blinker ) {
      leds_L[i] = CRGB(255,130,0);
      leds_R[i] = CRGB(255,130,0);
    } else {
      leds_L[i] = CRGB(0,0,0);
      leds_R[i] = CRGB(0,0,0);      
    }
  }
  
  for(int i = 0; i < NUM_LEDS_B; i = i + 1) {
    leds_B[i] = CRGB(255,0,0);
  }

  if ( blinker ) {
    for(int i = 0; i < NUM_TURN_B; i = i + 1) {
      if (i < NUM_TURN_B) {
        leds_B[i] = CRGB(150,30,0);
      } else if (i >= NUM_LEDS_B-NUM_TURN_B) {
  //      leds[i] = CRGB(255,90,0);
      }
    }
  }
  FastLED.show();
}
