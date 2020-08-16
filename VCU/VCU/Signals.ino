#include "Signals.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void Signals::setup(void)
{
  FastLED.addLeds<WS2811, GPIO_LED_TURN, GRB>(leds_turn, NUM_LEDS_TURN);
  FastLED.addLeds<WS2811, GPIO_LED_BACK, GRB>(leds_back, NUM_LEDS_B);
  FastLED.setBrightness(10);
  delay(100);
  pinMode(GPIO_LB, OUTPUT);
  pinMode(GPIO_HB, OUTPUT);
  pinMode(GPIO_HORN_OUT, OUTPUT);
  pinMode(GPIO_EXTRA_12V, OUTPUT);
  digitalWrite(GPIO_LB, LOW);
  digitalWrite(GPIO_HB, LOW);
  digitalWrite(GPIO_HORN_OUT, LOW);
  digitalWrite(GPIO_EXTRA_12V, LOW);
}

void Signals::service(void)
{
  setLights();
  setHorn();
}

void Signals::restart(void)
{
  // TODO
}

void Signals::setLights(void)
{
  // headlights
//  digitalWrite(GPIO_LB, vehicle.controls.headlights);
  digitalWrite(GPIO_HB, vehicle.controls.headlights);

  // GPIO_EXTRA_12V
  digitalWrite(GPIO_EXTRA_12V, vehicle.controls.S1);

}

void Signals::setHorn(void)
{
  digitalWrite(GPIO_HORN_OUT, vehicle.controls.horn);
}
