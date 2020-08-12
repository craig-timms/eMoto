#ifndef VEHICLE_H
#define VEHICLE_H

struct Vehicle
{
    // enum state {N, P, R, D, L, S};
    int state = 0;
    int speed = 0;
    int acceleration = 0;
    int roll = 0;
    int pitch = 0;
    
    struct Battery
    {
        int voltage = 0;
        int current = 0;
        int contactor = 0;
        int discharge = 0;
        int precharge = 0;
    };
    
    struct MCU
    {
        int gear = 0;
        int throttle = 0;
        int voltage = 0;
        int current = 0;
        int RPM = 0;
        int temp = 0;
        int tempM1 = 0;
        int tempM2 = 0;
        int errors = 0;
    };
    
    struct Lights
    {
        int turn = 0;
        int brake = 0;
        int beams = 0;
        int angel = 0;
    };
    
    struct Controls
    {
        // enum gear { P, R, N, D, L};
        bool key = false;
        int gear = 0;
        int intensity = 0;
        int throttle = 0;
        int regen = 0;
        bool brake = false;
        int turn = 0;
        bool horn = false;
        bool headlights = false;
        int angel = 0;
        bool S1 = false;
        bool S2 = false;
        bool AUX1 = false;
        bool AUX2 = false;
        bool AUX3 = false;
        bool AUX4 = false;
    };

    Battery battery;
    MCU mcu;
    Lights lights;
    Controls controls;

};

#endif VEHICLE_H
