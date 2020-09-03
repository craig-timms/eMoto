#ifndef HIGHVOLTAGE_H
#define HIGHVOLTAGE_H

#include <Wire.h>
#include <Adafruit_ADS1015.h>

#define GAIN_VISO 4.1
 
Adafruit_ADS1015 ads1015;    // Construct an ads1015 at the default address: 0x48

class HighVoltage
{
    public:
        void setup( void );             // Done
        void restart( void );
        void service( void );

        void enable( bool );

//        int getThrottle( void )         { return throttleOUT; }      // returns (0-1000)
        // int getRPM( void )              { return RPM; }
        // int getCurrent( void )          { return currentMtr; }
        // int getVoltage( void )          { return voltageDC; }
        // int getTemp( void )             { return tempMtr; }
        // int getTempInv( void )          { return tempInv; }
        // int getState( void )            { return statusCmd; }

    private:

        void setContactor( bool );
        // void setContactor2( bool );
        void setPrecharge( bool );
        // void setPrecharge2( bool );
        void setBleed( bool );
        // void setBleed2( bool );
        // void setBleedT( bool );
        // void setBleedB( bool );
        void getBatteryInfo( void );

        int previous_state = 0;
        int current_state = 0;
        int next_state = 0;
        
        int HVstatus = 0;

        int offDelayContactor = 400;

        // int tempInv = 0;
        // int tempMtr = 0;
        // char statusInv[ 16 + 1 ];
        // char statusCmd = 'N'; 
        // char statusFb = 'N'; 
        // int RPM = 0;
        // int currentMtr = 0;
        // int voltageDC = 0;
        // char ERR[ 16 + 1 ];
        // int HVstatus = 0;
                
        unsigned long HVcntrlTimer = 0;
        unsigned long HVonHoldoff = 10000;
        unsigned long HVoffHoldoff = 10000;

};

#endif HIGHVOLTAGE_H
