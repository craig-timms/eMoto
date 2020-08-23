#ifndef DASH_H
#define DASH_H

#include "FastLED.h"

class Dash
{
    public:
        void setup( void );
        void restart( void );
        void service( void );

    private:
        void setLEDS( void );
        void screenSetup( void );

        void screen1( int );
        void drawBitmap1( void );
        void drawBitmap2( void );

        bool key = false;
        int throttle = 0;
        int regen = 0;
        int headlights = 0;
        int turn = 0;
        bool horn = false;
        int motorTemp1 = 0;
        int motorTemp2 = 0;
        int expander = 0;

        CRGB leds_dash[NUM_LEDS_DISPLAY];

};

#endif DASH_H
