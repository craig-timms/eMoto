#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <ESP32CAN.h>
#include <CAN_config.h>

#define CANtx 17
#define CANrx 16
#define GPIO_throttleIN 39   // throttle
#define GPIO_keyIN 36        // throttle
#define GPIO_throttleOUT 23  // throttle
CAN_device_t CAN_cfg;

class Motor
{
    public:
        void setup( void );             // Done
        void restart( void );
        void service( void );

        int getThrottle( void )         {return throttleOUT; }      // returns (0-1000)
        int getRPM( void )              { return RPM; }
        int getCurrent( void )          { return currentMtr; }
        int getVoltage( void )          { return voltageDC; }
        int getTemp( void )             { return tempMtr; }
        int getTempInv( void )          { return tempInv; }
        int getState( void )            { return statusCmd; }
        // TODO: function to make sure set cmd == fb state
//        char getErrors( void )          { return ERR; }

    private:

        void readCAN( void );
        int readThrottle( void );          // read ADC (0-1000)
        void writeThrottle( void );         // send to motor (0-1000)

        // CAN_device_t CAN_cfg;               // CAN Config
        unsigned long previousMillis = 0;   // will store last time a CAN Message was send
        const int interval = 1000;          // interval at which send CAN Messages (milliseconds)
        const int rx_queue_size = 10;       // Receive Queue size

        int throttleIN = 0;
        int tINf[6] = {0, 0, 0, 0, 0, 0};
        int tINi = 0;
        int throttleOUT = 0;

        int throttleRead = 0;
        int tempInv = 0;
        int tempMtr = 0;
        char statusInv[ 16 + 1 ];
        char statusCmd = 'N'; 
        char statusFb = 'N'; 
        int RPM = 0;
        int currentMtr = 0;
        int voltageDC = 0;
        char ERR[ 16 + 1 ];

};

#endif MOTOR_H
