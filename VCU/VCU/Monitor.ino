#include "Monitor.h"
// #include "Vehicle.h"

#define ADC_THROTTLE_IN     33
#define POT_OFFSET          850
#define throttle_ave_n      8
int potValue = 0;
int throttle_ave = 0;
int throttle_i = 0;
int throttle_sum = 0;
int throttle_filter[throttle_ave_n] = {0};

void Monitor::setup(void)
{
    pinMode(GPIO_KEY_IN, INPUT_PULLDOWN);
    pinMode(GPIO_R_IN, INPUT_PULLDOWN);
    pinMode(GPIO_L_IN, INPUT_PULLDOWN);
    pinMode(GPIO_HORN_IN, INPUT_PULLDOWN);
    pinMode(GPIO_HL_IN, INPUT_PULLDOWN);
    pinMode(GPIO_WAKEUP_OUT, OUTPUT);
}

void Monitor::restart(void)
{
    // TODO
}

void Monitor::service(void)
{
    readControls();
    delay(10);
    shareData();
}

void Monitor::readControls(void)
{
    key = false;
    key = digitalRead(GPIO_KEY_IN);

    // throttle
    throttle_i += 1;
    if (throttle_i >= (throttle_ave_n))
    {
        throttle_i = 0;
    }
    potValue = (analogRead(ADC_THROTTLE_IN) - POT_OFFSET) * 1000;
    throttle_sum = throttle_sum - throttle_filter[throttle_i] + potValue;
    throttle_ave = throttle_sum / throttle_ave_n / 2212;
    throttle_filter[throttle_i] = potValue;
    if (throttle_ave < 5) { throttle_ave = 0; } 
    if (throttle_ave > 998) { throttle_ave = 1000; } 
    throttle = throttle_ave;

    regen = analogRead(ADC_REGEN);

    headlights = false;
    headlights = digitalRead(GPIO_HL_IN);
    // int HLanalog = analogRead(ADC_HL_IN);
    // if (HLanalog < 30)
    // {
    //     headlights = 0;
    // }
    // else if (HLanalog < 160)
    // {
    //     headlights = 1;
    // }
    // else
    // {
    //     headlights = 2;
    // }

    turn = 0;
    if (digitalRead(GPIO_L_IN))
    {
        turn = 1;
    }
    else if (digitalRead(GPIO_R_IN))
    {
        turn = 2;
    }
    else
    {
        turn = 0;
    }
    
    horn = false;
    horn = digitalRead(GPIO_HORN_IN);
}

void Monitor::shareData(void)
{
    vehicle.controls.throttle = throttle;
    vehicle.controls.turn = turn;
    vehicle.controls.horn = horn;
    vehicle.controls.headlights = headlights;
    vehicle.controls.key = key;
}
