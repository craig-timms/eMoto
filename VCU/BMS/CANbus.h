#ifndef CANBUS_H
#define CANBUS_H

// #include <ESP32CAN.h>
// #include <CAN_config.h>
#include <CAN.h>

// CAN_device_t CAN_cfg;               // CAN Config

unsigned long tMsg = millis();
unsigned long tMsgPeriod = 100;
unsigned long period = 2000;
unsigned long timerOBC = millis();

class CANbus
{
    public:
        void setup( void );
        void restart( void );
        void service( void );
        void readBus( void );
        
        void sendControls( void );
        void getControls( uint8_t msgIn[] );

        // TODO
        void getMCU1( uint8_t msgIn[] );
        void getMCU2( uint8_t msgIn[] );
        void getCharger( uint8_t msgIn[] );
        void getBattery( uint8_t msgIn[] );

        void sendCharger( bool, int, int, char );
        
        int getcVoltage( void )         { return cVoltage; }      
        int getcCurrent( void )         { return cCurrent; }
        bool getcCharging( void )        { return cCharging; }
        bool getcErrorTemp( void )       { return cErrorTemp; }
        bool getcErrorVac( void )        { return cErrorVac; }
        bool getcErrorHW( void )         { return cErrorHW; }
        bool getcErrorCom( void )        { return cErrorCom; }

        int getmRPM( void )             { return mRPM; }
        int getmCurrent( void )         { return mCurrent; }
        int getmVoltage( void )         { return mVoltage; }
        int getmTemp( void )            { return mTemp; }
        int getmTempI( void )           { return mTempI; }
        int getmState( void )           { return mGearFb; }
    
        bool getOnline(void);
        void sendOnline(void);

    private:

        int mRPM = 0;
        int mCurrent = 0;
        int mVoltage = 0;
        int mThrottle = 0;
        char mERR[ 16 + 1 ] = {};
        int mTemp = 0;
        int mTempI = 0;
        char mGearCmd = 'N'; 
        char mGearFb = 'N'; 

        int cVoltage = 0;
        int cCurrent = 0;
        bool cCharging = false;
        bool cErrorTemp = true;
        bool cErrorVac = true;
        bool cErrorHW = true;
        bool cErrorCom = true;

        unsigned long previousMillis = 0;   // will store last time a CAN Message was send
        const int interval = 1000;          // interval at which send CAN Messages (milliseconds)
        const int rx_queue_size = 10;       // Receive Queue size

};

#endif CANBUS_H
