#include "FastLED.h"

#define GPIO_LED_DASH   14
#define NUM_LEDS_DASH   5

CRGB leds_dash[NUM_LEDS_DASH];

void setup() {
  FastLED.addLeds<WS2811, GPIO_LED_DASH, GRB>(leds_dash, NUM_LEDS_DASH);
  FastLED.setBrightness(20);
  delay(100);
}

void loop() {
  for (int i = 0; i < NUM_LEDS_DASH; i = i + 1)
  {
    leds_dash[i] = CRGB(255, 0, 0);
  }
  FastLED.show();
  delay(1000);
  for (int i = 0; i < NUM_LEDS_DASH; i = i + 1)
  {
    leds_dash[i] = CRGB(0, 255, 0);
  }
  FastLED.show();
  delay(1000);
  for (int i = 0; i < NUM_LEDS_DASH; i = i + 1)
  {
    leds_dash[i] = CRGB(0, 0, 255);
  }
  FastLED.show();
  delay(1000);


}
