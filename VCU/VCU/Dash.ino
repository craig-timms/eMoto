#include "Dash.h"
#include "Bitmaps.h"


//NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> leds_dash(NUM_LEDS_DASH, );


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

//NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> leds_dash(NUM_LEDS_DASH, GPIO_LED_DASH);
// NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> leds_back(NUM_LEDS_BACK, GPIO_LED_BACK);

void Dash::setup(void)
{
  // FastLED.addLeds<WS2811, GPIO_LED_DASH, GRB>(leds_dash, NUM_LEDS_DASH);
  // FastLED.setBrightness(10);
//  leds_dash.Begin();
//  leds_dash.Show();
//  delay(100);

//  setWhite();
  delay(100);

  gpioExpander.setup();
  delay(100);

  screenSetup();
  // TODO - setup for screen
  // TODO - setup for GPIO EXTENSION
}

void Dash::setWhite(void)
{
//  for ( uint8_t i = 0; i < NUM_LEDS_DASH; i++ ) {
//    leds_dash.SetPixelColor(i, RgbwColor(0, 0, 0, 50));
//  }
//  leds_dash.Show();
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
  delay(2000);

  // TODO - setup for screen
  // TODO - setup for GPIO EXTENSION
}

void Dash::restart(void)
{
  // TODO
}

void Dash::service(void)
{
  if ( vehicle.controls.state==0 ) {
    setLEDS( 0 );
    screen1(3);
    screen2(3);
  } else if ( vehicle.controls.state==1 ) {
    setLEDS();
    screen1(1);
    screen2(2);
  }

  getDash();
}

void Dash::setLEDS( int state )
{
  if ( state == 0 ) {
    for ( uint8_t i = DASH_LED_START; i < DASH_LED_END; i++ ) {
      leds.SetPixelColor(i, RgbwColor(70, 30, 0, 0));
    }
  } else if ( state == 1 ) {
    // BMS 1st LED
    if ( !vehicle.battery.online ) {
      leds.SetPixelColor(DASH_LED_1, RgbwColor(70, 30, 0, 0));
    } else if ( vehicle.battery.fault ) {
      leds.SetPixelColor(DASH_LED_1, RgbwColor(70, 0, 0, 0));
    } else {
      leds.SetPixelColor(DASH_LED_1, RgbwColor(0, 70, 0, 0));
    }
    
    // MCU 2nd LED
    if ( !vehicle.mcu.online ) {
      leds.SetPixelColor(DASH_LED_2, RgbwColor(70, 30, 0, 0));
    } else if ( vehicle.mcu.fault ) {
      leds.SetPixelColor(DASH_LED_2, RgbwColor(70, 0, 0, 0));
    } else {
      leds.SetPixelColor(DASH_LED_2, RgbwColor(0, 70, 0, 0));
    }
    
    // OBC 3rd LED
    if ( !vehicle.charger.online ) {
      leds.SetPixelColor(DASH_LED_3, RgbwColor(70, 30, 0, 0));
    } else if ( vehicle.charger.fault ) {
      leds.SetPixelColor(DASH_LED_3, RgbwColor(70, 0, 0, 0));
    } else {
      leds.SetPixelColor(DASH_LED_3, RgbwColor(0, 70, 0, 0));
    }
    
    // for ( uint8_t i = DASH_LED_4; i < DASH_LED_END; i++ ) {
    //   leds.SetPixelColor(i, RgbwColor(70, 30, 0, 0));
    // }
  }
  leds.Show();
}

void Dash::getDash(void)
{
  expander = gpioExpander.readArr();

  // Gear
  if (bitRead(expander, 0) == 1)
  {
    vehicle.controls.gear = 4; // L
  }
  else if (bitRead(expander, 15) == 1)
  {
    vehicle.controls.gear = 3; // D
  }
  else if (bitRead(expander, 14) == 1)
  {
    vehicle.controls.gear = 2; // N
  }
  else if (bitRead(expander, 13) == 1)
  {
    vehicle.controls.gear = 1; // R
  }
  else if (bitRead(expander, 12) == 1)
  {
    vehicle.controls.gear = 0; // P
  }

  // Intensity
  if (bitRead(expander, 3) == 1)
  {
    vehicle.controls.intensity = 1;
  }
  else if (bitRead(expander, 4) == 1)
  {
    vehicle.controls.intensity = 2;
  }
  else if (bitRead(expander, 5) == 1)
  {
    vehicle.controls.intensity = 3;
  }
  else if (bitRead(expander, 6) == 1)
  {
    vehicle.controls.intensity = 4;
  }
  else if (bitRead(expander, 7) == 1)
  {
    vehicle.controls.intensity = 5;
  }

  // Switches
  if (bitRead(expander, 2) == 1)
  {
    vehicle.controls.S1 = true;
  }
  else
  {
    vehicle.controls.S1 = false;
  }

  if (bitRead(expander, 1) == 1)
  {
    vehicle.controls.S2 = true;
  }
  else
  {
    vehicle.controls.S2 = false;
  }

  if (bitRead(expander, 8) == 1)
  {
    vehicle.controls.AUX1 = true;
  }
  else
  {
    vehicle.controls.AUX1 = false;
  }

  if (bitRead(expander, 9) == 1)
  {
    vehicle.controls.AUX2 = true;
  }
  else
  {
    vehicle.controls.AUX2 = false;
  }

  if (bitRead(expander, 10) == 1)
  {
    vehicle.controls.AUX3 = true;
  }
  else
  {
    vehicle.controls.AUX3 = false;
  }

  if (bitRead(expander, 11) == 1)
  {
    vehicle.controls.AUX1 = true;
  }
  else
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
    display.setTextSize(1);
    display.print(F("Level:    "));
    display.setTextSize(2);
    display.println(vehicle.controls.intensity);
    display.display();
  }
  else if (state == 0)
  {
    drawBitmap1();
  }
  else if (state == 3)
  {
    // Not in park at startup
    display.setTextSize(4);
    display.println(F("GO TO"));
    display.println(F("PARK"));
  }
}

void Dash::screen2( int state )
{
  display2.clearDisplay();
  display2.setCursor(0, 0);
  display2.setTextSize(2);
  display2.setTextColor(WHITE); // Draw 'inverse' text

  if (state == 1)
  {
    // Battery Voltage 1
    display2.setTextSize(1);
    display2.print(F("Turn:   "));
    display2.setTextSize(2);
    if (vehicle.controls.turn == 1)
    {
      display2.println(F("L"));
    }
    else if ( vehicle.controls.turn == 2 )
    {
      display2.println(F("R"));
    }
    else
    {
      display2.println(  );
    }

    // Battery Voltage 2
    display2.setTextSize(1);
    display2.print(F("Lights: "));
    display2.setTextSize(2);
    if ( vehicle.controls.headlights )
    {
      display2.println(F("H"));
    }
    else
    {
      display2.println(F("L"));
    }
    display2.setTextSize(1);
    display2.print(F("Horn:   "));
    display2.setTextSize(2);
    if ( vehicle.controls.horn )
    {
      display2.println(F("HONK"));
    } else
    {
      display2.println();
    }
    // Key
    display2.setTextSize(1);
    display2.print(F("Key:    "));
    display2.setTextSize(2);
    if ( vehicle.controls.key )
    {
      display2.println(F("ON"));
    } else
    {
      display2.println(F("OFF"));
    }
    display2.display();
  } else if (state == 2)
  {
    // MCU voltage and current
    display2.setTextSize(2);
    int vtemp = vehicle.mcu.voltage;
    if ( vtemp <10 ){
      display2.print(F("  "));
    } else if ( vtemp < 100 ) {
      display2.print(F(" "));
    }
    display2.print( vtemp );
    display2.setTextSize(1);
    display2.print(F("V"));
    display2.setTextSize(2);
    display2.print(F(" "));
    int itemp = vehicle.mcu.current;
    if ( itemp <10 ) {
      display2.print(F("  "));
    } else if ( itemp < 100 ) {
      display2.print(F(" "));
    }
    display2.print( itemp );
    display2.setTextSize(1);
    display2.print(F("A"));
    display2.setTextSize(2);
    display2.println();
    
    // MCU direction
    display2.setTextSize(1);
    display2.println();
    display2.print(F("CMD: "));
    // display2.setTextSize(2);
    uint8_t dirtemp = (uint8_t)vehicle.mcu.rCmd;
    if ( dirtemp == 0 ){
      display2.print(F("N"));
    } else if ( vtemp == 1 ) {
      display2.print(F("F"));
    } else if ( vtemp == 2 ) {
      display2.print(F("R"));
    } else if ( vtemp == 3 ) {
      display2.print(F("?"));
    } else {
      display2.print( dirtemp );
    }
    display2.setTextSize(1);
    display2.print(F("  FB: "));
    // display2.setTextSize(2);
    dirtemp = vehicle.mcu.rDirection;
    if ( dirtemp == 0 ){
      display2.print(F("N"));
    } else if ( vtemp == 1 ) {
      display2.print(F("F"));
    } else if ( vtemp == 2 ) {
      display2.print(F("R"));
    } else if ( vtemp == 3 ) {
      display2.print(F("?"));
    } else {
      display2.print( dirtemp );
    }
    display2.println();
    
    // Errors
    display2.setTextSize(1);
//    display2.println(F("Errors:   "));
    display2.println();
    for ( int i = 0; i<8; i++ ) {
      if ( bitRead(vehicle.mcu.errorsA,7-i) == 1 ) {
        display2.print(F("1"));
      } else {
        display2.print(F("0"));
      }
      // Serial.print( bitRead(vehicle.mcu.errorsA,7-i) == 1 );
      // Serial.print( " " );
    }
    // Serial.println();
    display2.print(F(" - "));
    for ( int i = 0; i<8; i++ ) {
      if ( bitRead(vehicle.mcu.errorsB,7-i) ) {
        display2.print(F("1"));
      } else {
        display2.print(F("0"));
      }
    }
    display2.println();
    
    // Switch Status
    display2.setTextSize(1);
//    display2.println(F("Switches: "));
    display2.println();
    // Boost
    if ( bitRead(vehicle.mcu.rSwStatus,7) ) {
      display2.setTextColor(BLACK, WHITE);
    }
    display2.print(F("BST"));
    display2.setTextColor(WHITE);
    display2.print(F(" "));
    // Foot
    if ( bitRead(vehicle.mcu.rSwStatus,6) ) {
      display2.setTextColor(BLACK, WHITE);
    }
    display2.print(F("FT"));
    display2.setTextColor(WHITE);
    display2.print(F(" "));
    // Forward
    if ( bitRead(vehicle.mcu.rSwStatus,5) ) {
      display2.setTextColor(BLACK, WHITE);
    }
    display2.print(F("F"));
    display2.setTextColor(WHITE);
    display2.print(F(" "));
    // Backward
    if ( bitRead(vehicle.mcu.rSwStatus,4) ) {
      display2.setTextColor(BLACK, WHITE);
    }
    display2.print(F("R"));
    display2.setTextColor(WHITE);
    display2.print(F(" "));
    // 12V Brake
    if ( bitRead(vehicle.mcu.rSwStatus,3) ) {
      display2.setTextColor(BLACK, WHITE);
    }
    display2.print(F("BK"));
    display2.setTextColor(WHITE);
    display2.print(F(" "));
    // Hall C
    if ( bitRead(vehicle.mcu.rSwStatus,2) ) {
      display2.setTextColor(BLACK, WHITE);
    }
    display2.print(F("C"));
    display2.setTextColor(WHITE);
    display2.print(F(" "));
    // Hall B
    if ( bitRead(vehicle.mcu.rSwStatus,1) ) {
      display2.setTextColor(BLACK, WHITE);
    }
    display2.print(F("B"));
    display2.setTextColor(WHITE);
    display2.print(F(" "));
    // Hall A
    if ( bitRead(vehicle.mcu.rSwStatus,0) ) {
      display2.setTextColor(BLACK, WHITE);
    }
    display2.print(F("A"));
    display2.setTextColor(WHITE);
    display2.print(F(" "));
//    display.println();

    display2.display();
  } else if (state == 0)
  {
    drawBitmap2();
  }
  else if (state == 3)
  {
    // Not in park at startup
    display2.setTextSize(4);
    display2.println(F("GO TO"));
    display2.println(F("PARK"));
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
  display2.clearDisplay();

  display2.drawBitmap(
    0,
    0,
    bitmapCBE, 128, 64, 1);
  //  display2.drawBitmap(0.0, myBitmapBitmap, 128.64, WHITE);
  display2.display();
  delay(1000);
}
