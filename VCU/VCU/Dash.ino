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
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MCPAddress 0x20

#define GP0 0x00     // DATA PORT REGISTER 0
#define GP1 0x01     // DATA PORT REGISTER 1
#define OLAT0 0x02   // OUTPUT LATCH REGISTER 0
#define OLAT1 0x03   // OUTPUT LATCH REGISTER 1
#define IPOL0 0x04   // INPUT POLARITY PORT REGISTER 0
#define IPOL1 0x05   // INPUT POLARITY PORT REGISTER 1
#define IODIR0 0x06  // I/O DIRECTION REGISTER 0
#define IODIR1 0x07  // I/O DIRECTION REGISTER 1
#define INTCAP0 0x08 // INTERRUPT CAPTURE REGISTER 0
#define INTCAP1 0x09 // INTERRUPT CAPTURE REGISTER 1
#define IOCON0 0x0A  // I/O EXPANDER CONTROL REGISTER 0
#define IOCON1 0x0B  // I/O EXPANDER CONTROL REGISTER 1

void Dash::setup(void)
{
  FastLED.addLeds<WS2811, GPIO_LED_DASH, GRB>(leds_dash, NUM_LEDS_DASH);
  FastLED.setBrightness(10);
  delay(100);
  gpioExpander.setup();
  delay(100);
  screenSetup();
  // TODO - setup for screen
  // TODO - setup for GPIO EXTENSION
}

void Dash::screenSetup(void)
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D))
  { // Address 0x3D 0x7A for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  delay(100);
  if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("Second SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  delay(100);

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  // display.display();
  // display2.display();
  // delay(1000);

  drawBitmap2();
  screen1(0);
  delay(100);

  // TODO - setup for screen
  // TODO - setup for GPIO EXTENSION
}

void Dash::restart(void)
{
  // TODO
}

void Dash::service(void)
{
  setLEDS();
  screen1(1);
//  screen2();

  getDash();
}

void Dash::setLEDS(void)
{
  for (int i = 0; i < NUM_LEDS_DASH; i = i + 1)
  {
    leds_dash[i] = CRGB(255, 0, 0);
  }
  FastLED.show();
}

void Dash::getDash(void)
{
  expander = gpioExpander.readArr();
  for (int i = 15; i >= 0; i--)
  {
    Serial.print(bitRead(expander, i));
  }
  Serial.print(" - ");

    // Gear
  if ( bitRead(expander, 0) == 1 ) {
      vehicle.controls.gear = 4;    // L
  } else if ( bitRead(expander, 15) == 1 ) {
        vehicle.controls.gear = 3;  // D
  } else if ( bitRead(expander, 14) == 1 ) {
        vehicle.controls.gear = 2;  // N
  } else if ( bitRead(expander, 13) == 1 ) {
        vehicle.controls.gear = 1;  // R
  } else if ( bitRead(expander, 12) == 1 ) {
        vehicle.controls.gear = 0;  // P  
  }

    // Intensity
  if ( bitRead(expander, 3) == 1 ) {
      vehicle.controls.intensity = 1;  
  } else if ( bitRead(expander, 4) == 1 ) {
        vehicle.controls.intensity = 2;
  } else if ( bitRead(expander, 5) == 1 ) {
        vehicle.controls.intensity = 3;
  } else if ( bitRead(expander, 6) == 1 ) {
        vehicle.controls.intensity = 4;
  } else if ( bitRead(expander, 7) == 1 ) {
        vehicle.controls.intensity = 5;  
  }

  // Switches
  if ( bitRead(expander, 2) == 1 ) { 
        vehicle.controls.S1 = true;  
  } else
  {
     vehicle.controls.S1 = false;  
  }
  
  if ( bitRead(expander, 1) == 1 ) { 
        vehicle.controls.S2 = true;  
  } else
  {
     vehicle.controls.S2 = false;  
  }
  
  if ( bitRead(expander, 8) == 1 ) { 
        vehicle.controls.AUX1 = true;  
  } else
  {
     vehicle.controls.AUX1 = false;  
  }
  
  if ( bitRead(expander, 9) == 1 ) { 
        vehicle.controls.AUX2 = true;  
  } else
  {
     vehicle.controls.AUX2 = false;  
  }
  
  if ( bitRead(expander, 10) == 1 ) { 
        vehicle.controls.AUX3 = true;  
  } else
  {
     vehicle.controls.AUX3 = false;  
  }
  
  if ( bitRead(expander, 11) == 1 ) { 
        vehicle.controls.AUX1 = true;  
  } else
  {
     vehicle.controls.AUX1 = false;  
  }
  


    /*
        15 - D
        14 - N
        13 - R
        12 - P
        11 - AUX4
        10 - AUX3
        9  - AUX2
        8  - AUX1
        7  - 5
        6  - 4
        5  - 3
        4  - 2
        3  - 1
        2  - S1
        1  - S2
        0  - L
    */

}

void Dash::shareData(void)
{
}

void Dash::setDash(void)
{
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
    display.print(F("Throttle: "));
    display.setTextSize(2);
    display.println(vehicle.controls.throttle);
    // Battery Voltage 2
    display.setTextSize(1);
    display.print(F("Gear:     "));
    display.setTextSize(2);
    display.println(vehicle.controls.gear);
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

void Dash::screen2(void)
{
  // if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  // { // Address 0x3D for 128x64
  //     Serial.println(F("Second SSD1306 allocation failed"));
  //     for (;;)
  //         ; // Don't proceed, loop forever
  // }

  display2.clearDisplay();
  display2.display();
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
  display2.clearDisplay();

  display2.drawBitmap(
    0,
    0,
    bitmapCBE, 128, 64, 1);
  //  display2.drawBitmap(0.0, myBitmapBitmap, 128.64, WHITE);
  display2.display();
  delay(1000);
}
