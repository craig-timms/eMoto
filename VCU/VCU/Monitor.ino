#include "Monitor.h"
// #include "Vehicle.h"

void Monitor::setup(void)
{
    pinMode(GPIO_KEY_IN, INPUT);
    pinMode(GPIO_R_IN, INPUT);
    pinMode(GPIO_L_IN, INPUT);
    pinMode(GPIO_HORN_IN, INPUT);
    pinMode(GPIO_WAKEUP_OUT, OUTPUT);
}

void Monitor::restart(void)
{
    // TODO
}

void Monitor::service(void)
{
    readControls();
}

void Monitor::readControls(void)
{
    key = digitalRead(GPIO_KEY_IN);
    throttle = analogRead( ADC_THROTTLE_IN );
    regen = analogRead(ADC_REGEN);

    int HLanalog = analogRead(ADC_HL_IN);
    if ( HLanalog < 30 ) {
        headlights = 0;
    } else if ( HLanalog < 160 )
    {
        headlights = 1;
    } else
    {
        headlights = 2;
    }
    
    if ( digitalRead(GPIO_L_IN) ) {
        turn = 1;
    } else if ( digitalRead(GPIO_R_IN) )
    {
        turn = 2;
    } else
    {
        turn = 0;
    }
    horn = digitalRead(GPIO_HORN_IN);

}

void Monitor::shareData(void)
{
    vehicle.Controls.throttle = throttle;
}