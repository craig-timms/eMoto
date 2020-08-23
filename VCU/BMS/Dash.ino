#include "Dash.h"
#include "Bitmaps.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include "Vehicle.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void Dash::setup(void)
{
  // LEDs
  FastLED.addLeds<WS2811, GPIO_LED_DISPLAY, GRB>(leds_dash, NUM_LEDS_DISPLAY);
  FastLED.setBrightness(10);
  delay(100);

  // Screen
  screenSetup();
  delay(100);
  
}

void Dash::screenSetup(void)
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D 0x7A for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  delay(100);

  screen1(0);
  delay(2000);

}

void Dash::restart(void)
{
  // TODO
}

void Dash::service(void)
{
  setLEDS();
  screen1(1);

}

void Dash::setLEDS(void)
{
  for (int i = 0; i < NUM_LEDS_DISPLAY; i = i + 1)
  {
    leds_dash[i] = CRGB(255, 0, 0);
  }
  FastLED.show();
}

void Dash::screen1(int state)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE); // Draw 'inverse' text

  if (state == 1)
  {
    // Battery Voltage 1
    display.setTextSize(1);
    display.print(F("Gear:     "));
    display.setTextSize(2);
    display.println(vehicle.controls.gear);
    //
    display.setTextSize(1);
    display.print(F("Throttle: "));
    display.setTextSize(2);
    display.println(vehicle.controls.throttle);
    // Battery Voltage 2
    display.setTextSize(1);
    display.print(F("Regen:    "));
    display.setTextSize(2);
    display.println(vehicle.controls.regen);
    // display.print(vR[1], 2);
    // display.setTextSize(1);
    // display.print(F("V "));
    // display.setTextSize(2);
    // display.print(F("\n"));       // new line
    // // Battery Voltage 3
    display.setTextSize(1);
    display.print(F("Level:    "));
    display.setTextSize(2);
    display.println(vehicle.controls.intensity);
    // display.print(vR[2], 2);
    // display.setTextSize(1);
    // display.print(F("V "));
    // display.setTextSize(2);
    // // Battery Voltage 4
    // display.print(vR[3], 2);
    // display.setTextSize(1);
    // display.print(F("V "));
    // display.setTextSize(2);
    // display.print(F("\n"));       // new line
    // // Battery Voltage 5
    // display.print(vR[4], 2);
    // display.setTextSize(1);
    // display.print(F("V "));
    // display.setTextSize(2);
    // display.print(F("\n"));       // new line
    // display.display();
    display.display();
  }
  else if (state == 0)
  {
    drawBitmap1();
  }
}
void Dash::drawBitmap1(void)
{
  display.clearDisplay();

  display.drawBitmap(
    0,
    0,
    bitmapPB, 128, 64, 1);
  //  display.drawBitmap(0.0, myBitmapBitmap, 128.64, WHITE);
  display.display();
  delay(1000);
}

void Dash::drawBitmap2(void)
{
  display.clearDisplay();

  display.drawBitmap(
    0,
    0,
    bitmapCBE, 128, 64, 1);
  //  display2.drawBitmap(0.0, myBitmapBitmap, 128.64, WHITE);
  display.display();
  delay(1000);
}
