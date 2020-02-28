#ifdef CHARGER_H
#define CHARGER_H

#define GPIO_precharge 2       // TODO
#define GPIO_contactor 21       // TODO
#define GPIO_discharge 4       // TODO

class Charger
{
    public:
        void service();
        void setHV( int );
        void getHV( void );
        
}

#endif CHARGER_H