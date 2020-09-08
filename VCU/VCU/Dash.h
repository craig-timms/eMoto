#ifndef DASH_H
#define DASH_H

//#include <NeoPixelBus.h>
#include "MCP23016.h"

MCP23016 gpioExpander;

class Dash
{
    public:
        void setup( void );
        void restart( void );
        void service( void );

    private:
        void setLEDS( void );
        void setWhite( void );
        void shareData( void );
        void screenSetup( void );
        void setDash( void );
        
        void getDash( void );

        void screen1( int );
        void screen2( int );
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

        // CRGB leds_dash[NUM_LEDS_DASH];

};

#endif DASH_H
