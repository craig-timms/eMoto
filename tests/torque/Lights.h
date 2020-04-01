#ifndef LIGHTS_H
#define LIGHTS_H

#include "FastLED.h"

#define ADC_TURN_BT 27
#define ADC_HEADLIGHTS_BT 26
#define GPIO_HL_OUT 13
#define GPIO_BRIGHTS_OUT 22
#define GPIO_ANGEL = 13

#define DATA_PIN_B 5    // front
#define DATA_PIN_L 12   // left
#define DATA_PIN_R 14   // right
#define DATA_PIN_A 22   // angel

#define NUM_LEDS_B 12
#define NUM_TURN_B 4
#define NUM_LEDS_turn 4
#define NUM_LEDS_ANGEL 12

#define CLOCK_PIN 13    // Actually probs don't need

// CRGB leds_B[NUM_LEDS_B];
// CRGB leds_L[NUM_LEDS_turn];
// CRGB leds_R[NUM_LEDS_turn];
// CRGB leds_R[NUM_LEDS_turn];

class Lights
{
    public:
        void setup( void );
        void restart( void );
        void service( void );

        void setTurn( int );                // 0-off 1-left 2-right
        void setBrake( bool );
        void setAngel( bool );              // R, G, B
        void setAngelRGB( int, int, int );  // R, G, B

    private:
        boolean angel = false;
        boolean headlight = false;
        boolean brights = false;
        boolean braking = false;
        boolean right = false;
        boolean left = false;
        boolean blinker = false;
        unsigned long blinkerTimer = 0;
        unsigned long blinkerPeriod = 800;

        int brightness = 20;
        int angelRGB[3] = {250, 250, 250};

        CRGB leds_B[NUM_LEDS_B];
        CRGB leds_L[NUM_LEDS_turn];
        CRGB leds_R[NUM_LEDS_turn];
        CRGB leds_A[NUM_LEDS_ANGEL];

};

#endif LIGHTS_H
