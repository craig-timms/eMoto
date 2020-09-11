#include "Signals.h"

NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> leds(NUM_LEDS, GPIO_LED_TURN);
// NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> leds_back(NUM_LEDS_BACK, GPIO_LED_BACK);


// const uint16_t PixelCount = 4;
// const uint8_t PixelPin = 15;
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void Signals::setup(void)
{
  leds.Begin();
  leds.Show();
  delay(10);
//  leds_back.Begin();
//  leds_back.Show();
  // delay(10);
//
  setWhite( true, true, true );
  leds.Show();

  pinMode(GPIO_LB, OUTPUT);
  pinMode(GPIO_HB, OUTPUT);
  pinMode(GPIO_HORN_OUT, OUTPUT);
  digitalWrite(GPIO_LB, LOW);
  digitalWrite(GPIO_HB, LOW);
  digitalWrite(GPIO_HORN_OUT, LOW);

}

void Signals::service(void)
{
  if ( millis() > (turnTimer+turnPeriod) ) {
    turnEn = !turnEn;
    turnTimer = millis();
  }

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

  vehicle.lights.DRL[4] = 20*(vehicle.controls.intensity-1);

  if ( (vehicle.controls.gear==1) ) {
    setWhite( 0, 0, true );
  }

  if ( (vehicle.controls.turn==0) ) {
    setLeft( true );
    setWhite( vehicle.lights.DRL[0], vehicle.lights.DRL[0], false );
  } else if ( (vehicle.controls.turn==1) && (turnEn) ) {
    setLeft( true );
    setWhite( false, vehicle.lights.DRL[0], false );
  } else if ( (vehicle.controls.turn==1) && (!turnEn) ) {
    setLeft( false );
    setWhite( false, vehicle.lights.DRL[0], false );
  } else if ( (vehicle.controls.turn==2) && (turnEn) ) {
    setRight( true );
    setWhite( vehicle.lights.DRL[0], false, false );
  } else if ( (vehicle.controls.turn==2) && (!turnEn) ) {
    setRight( false );
    setWhite( vehicle.lights.DRL[0], false, false );
  } 

  leds.Show();

}

void Signals::setHorn(void)
{
  digitalWrite(GPIO_HORN_OUT, vehicle.controls.horn);
}

void Signals::setLeft( bool on )
{
  uint8_t startLED = NUM_LEDS-NUM_LEDS_BACK;
  if ( on ) {
    for ( uint8_t i = (NUM_LEDS_TURN/2); i < NUM_LEDS_TURN; i++ ) {
      leds.SetPixelColor(i, RgbwColor(150, 60, 0, 0));
    }
    for ( uint8_t i = startLED; i < startLED+NUM_LEDS_BT; i++ ) {
     leds.SetPixelColor(i, RgbwColor(150, 60, 0, 0));
    }
  } else {
    for ( uint8_t i = (NUM_LEDS_TURN/2); i < NUM_LEDS_TURN; i++ ) {
      leds.SetPixelColor(i, RgbwColor(0, 0, 0, 0));
    }
    for ( uint8_t i = startLED; i < startLED+NUM_LEDS_BT; i++ ) {
     leds.SetPixelColor(i, RgbwColor(0, 0, 0, 0));
    }
  }
}

void Signals::setRight( bool on )
{
  uint8_t startLED = NUM_LEDS-NUM_LEDS_BT;
  if ( on ) {
    for ( uint8_t i = 0; i < (NUM_LEDS_TURN/2); i++ ) {
      leds.SetPixelColor(i, RgbwColor(150, 60, 0, 0));
    }
    for ( uint8_t i = startLED; i < NUM_LEDS; i++ ) {
     leds.SetPixelColor(i, RgbwColor(150, 60, 0, 0));
    }
  } else {
    for ( uint8_t i = 0; i < (NUM_LEDS_TURN/2); i++ ) {
      leds.SetPixelColor(i, RgbwColor(0, 0, 0, 0));
    }
    for ( uint8_t i = startLED; i < NUM_LEDS; i++ ) {
     leds.SetPixelColor(i, RgbwColor(0, 0, 0, 0));
    }
  }
}

void Signals::setWhite( bool left, bool right, bool back )
{
  uint8_t r = vehicle.lights.DRL[1];
  uint8_t g = vehicle.lights.DRL[2];
  uint8_t b = vehicle.lights.DRL[3];
  uint8_t w = vehicle.lights.DRL[4];

  if ( right ) {
    for ( uint8_t i = 0; i < (NUM_LEDS_TURN/2); i++ ) {
      leds.SetPixelColor(i, RgbwColor(r, g, b, w));
    }
  }

  if ( left ) {
    for ( uint8_t i = (NUM_LEDS_TURN/2); i < NUM_LEDS_TURN; i++ ) {
      leds.SetPixelColor(i, RgbwColor(r, g, b, w));
    }
  }

 if ( back ) {
   for ( uint8_t i = (NUM_LEDS-NUM_LEDS_BACK); i < NUM_LEDS; i++ ) {
     leds.SetPixelColor(i, RgbwColor(r, g, b, w));
   }
 }

}
