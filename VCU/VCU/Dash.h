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
        void shareData( void );
        void screenSetup( void );
        void setDash( void );
        void screen1( void );
        void screen2( void );

        bool key = false;
        int throttle = 0;
        int regen = 0;
        int headlights = 0;
        int turn = 0;
        bool horn = false;
        int motorTemp1 = 0;
        int motorTemp2 = 0;

};

#endif DASH_H
