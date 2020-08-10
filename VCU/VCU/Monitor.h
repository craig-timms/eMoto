#ifndef MONITOR_H
#define MONITOR_H

class Monitor
{
    public:
        void setup( void );
        void restart( void );
        void service( void );

    private:
        void readControls( void );
        void shareData( void );

        bool key = false;
        int throttle = 0;
        int regen = 0;
        int headlights = 0;
        int turn = 0;
        bool horn = false;
        int motorTemp1 = 0;
        int motorTemp2 = 0;

};

#endif MONITOR_H
