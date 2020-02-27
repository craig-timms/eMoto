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

#define NUM_LEDS 12
#define NUM_TURN 4

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.
#define DATA_PIN 22
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];

void setup() {
    // sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(2000);

    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
     FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);

    // FastLED.setBrightness(CRGB(255,255,255));
//    FastLED.setBrightness( CRGB(80,80,80) );
    FastLED.setBrightness( 100 );
}

void loop() {
//  FastLED.setBrightness( 100 );
  for(int i = 0; i < NUM_LEDS; i = i + 1) {
    leds[i] = CRGB(255,0,0);
  }
  FastLED.show();
  delay(1000);

//  FastLED.setBrightness( 200 );
  for(int i = 0; i < NUM_LEDS; i = i + 1) {
    if (i < NUM_TURN) {
      leds[i] = CRGB(150,30,0);
    } else if (i >= NUM_LEDS-NUM_TURN) {
//      leds[i] = CRGB(255,90,0);
    }
  }
  FastLED.show();
  delay(1000);
}
