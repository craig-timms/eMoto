#ifndef VEHICLE_H
#define VEHICLE_H

struct Vehicle
{
    int state;
    int speed;
    int acceleration;
    int roll;
    int pitch;
    
    struct Battery
    {
        int voltage;
        int current;
        int contactor;
        int discharge;
        int precharge;
    };
    
    struct MCU
    {
        int gear;
        int throttle;
        int voltage;
        int current;
        int RPM;
        int temp;
        int tempM1;
        int tempM2;
        int errors;
    };
    
    struct Lights
    {
        enum turn {OFF, R, L};
        int brake;
        enum beams {OFF, LOW, HIGH};
        int angel;
    };
    
    struct Controls
    {
        enum gear { P, R, N, D, L};
        int intensity;
        int throttle;
        int brake;
        int turn;
        int brake;
        int beams;
        int angel;
    };

    Battery battery;
    MCU mcu;
    Lights lights;
    Controls controls;

};

#endif VEHICLE_H