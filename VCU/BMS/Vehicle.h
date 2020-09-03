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
        bool discharge = false;
        bool precharge = false;
        bool HV = true;
        bool appPre = false;
        bool appBleed = false;
        bool appHV = false;
    };
    
    struct Charger
    {
        int vMax = 68;
        int iMax = 1;
        int errors = 0;
        bool enable = false;
        bool online = false;
        int rV = 0;
        int rI = 0;
        bool eTemp = false;
        bool eVac = false;
        bool eHW = false;
        bool eCom = false;
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
        int gear = 0;
        int throttle = 0;
        int regen = 0;
        int intensity = 0;
        int turn = 0;
        int angel = 0;
        bool key = false;
        bool brake = false;
        bool horn = false;
        bool headlights = false;
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
    Charger charger;

};

#endif VEHICLE_H
