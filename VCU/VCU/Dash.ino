#include "Dash.h"

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

void Dash::setup(void)
{
    FastLED.addLeds<WS2811, GPIO_LED_DASH, GRB>(leds_dash, NUM_LEDS_DASH);
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
    delay(1000);
    if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("Second SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    display2.display();
    delay(1000); // Pause for 1 seconds

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
    screen1();
    screen2();
}

void Dash::setLEDS(void)
{
    for (int i = 0; i < NUM_LEDS_DASH; i = i + 1)
    {
        leds_dash[i] = CRGB(255, 0, 0);
    }
}

void Dash::shareData(void)
{
    vehicle.Controls.throttle = throttle;
}

void Dash::setDash(void)
{
    vehicle.Controls.throttle = throttle;
}

void Dash::screen1(void)
{
    vehicle.Controls.throttle = throttle;
}

void Dash::screen2(void)
{
    vehicle.Controls.throttle = throttle;
}