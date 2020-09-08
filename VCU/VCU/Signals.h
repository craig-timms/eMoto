#ifndef SIGNALS_H
#define SIGNALS_H

#include <NeoPixelBus.h>

class Signals
{
    public:
        void setup( void );
        void restart( void );
        void service( void );

    private:
        void setLights( void );
        void setHorn( void );
        void setLeft( bool );
        void setRight( bool );

        void setWhite( bool, bool, bool );

        bool turnEn = false;
        unsigned long turnPeriod = 500;
        unsigned long turnTimer = 0;

};

#endif SIGNALS_H
