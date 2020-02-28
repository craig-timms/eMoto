#ifdef CANMESSAGE_H
#define CANMESSAGE_H

class CANmessage
{
    public:
        unit8_t msg[8];
        int ID;
        int time;
}

#endif CANMESSAGE_H