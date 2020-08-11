#ifndef SIGNALS_H
#define SIGNALS_H

#include "FastLED.h"

class Signals
{
    public:
        void setup( void );
        void restart( void );
        void read( void );

        void setHead( bool );
        void setBrake( bool );
        void setTurn( int );
        void setHorn( bool );

        void setBlinkerTime( int );

        int getLeft( void )         { return left; }      
        int getcRight( void )         { return right; }
        bool getcHigh( void )        { return high; }
        bool getcLow( void )       { return low; }
        bool getcHorn( void )        { return horn; }

    private:
        bool left = false;
        bool right = false;
        bool high = false;
        bool low = false;
        bool horn = false;

        unsigned long blinkerTimer = 0;
        unsigned long blinkerPeriod = 800;

        int brightness = 20;
        int angelRGB[3] = {250, 250, 250};

        CRGB leds_B[NUM_LEDS_B];
        CRGB leds_L[NUM_LEDS_turn];
        CRGB leds_R[NUM_LEDS_turn];
        CRGB leds_A[NUM_LEDS_ANGEL];

};

#endif SIGNALS_H
