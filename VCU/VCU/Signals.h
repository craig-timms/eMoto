#ifndef SIGNALS_H
#define SIGNALS_H

#include "FastLED.h"

class Signals
{
    public:
        void setup( void );
        void restart( void );
        void service( void );

    private:
        void setLights( void );
        void setHorn( void );

        CRGB leds_turn[NUM_LEDS_TURN];
        CRGB leds_back[NUM_LEDS_B];

};

#endif SIGNALS_H
