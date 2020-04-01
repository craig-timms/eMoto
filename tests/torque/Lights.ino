#include "Lights.h"

void Lights::setup(void)
{
    // sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(300);

    // Initiate all strands
    FastLED.addLeds<WS2811, DATA_PIN_B, GRB>(leds_B, NUM_LEDS_B);
    FastLED.addLeds<WS2811, DATA_PIN_L, GRB>(leds_L, NUM_LEDS_turn);
    FastLED.addLeds<WS2811, DATA_PIN_R, GRB>(leds_R, NUM_LEDS_turn);
    FastLED.addLeds<WS2811, DATA_PIN_A, GRB>(leds_A, NUM_LEDS_ANGEL);

    // Dim
    FastLED.setBrightness(20);

    blinkerTimer = millis();
}

void Lights::restart(void)
{
    blinkerTimer = millis();
    // TODO
}

void Lights::setTurn(int setT)
{
    if (setT == 1)
    {
        right = false;
        left = true;
        Serial.println("set left");
    }
    else if (setT == 2)
    {
        right = true;
        left = false;
        Serial.println("set right");
    }
    else
    {
        right = false;
        left = false;
        Serial.println("set none");
    }
}

void Lights::setBrake(bool setTo)
{
    braking = setTo;
}

void Lights::setAngel(bool setTo)
{
    angel = setTo;
}

void Lights::setAngelRGB(int setR, int setG, int setB)
{
    angelRGB[0] = setR;
    angelRGB[1] = setG;
    angelRGB[2] = setB;
}

void Lights::service(void)
{
    // Angel Eye
    if (angel)
    {
        for (int i = 0; i < NUM_LEDS_ANGEL; i = i + 1)
        {
            leds_A[i] = CRGB(angelRGB[0], angelRGB[1], angelRGB[2]);
        }
    }
    else
    {
        for (int i = 0; i < NUM_LEDS_ANGEL; i = i + 1)
        {
            leds_A[i] = CRGB(0, 0, 0);
        }
    }

    // Brake
    if (braking)
    {
        for (int i = 0; i < NUM_LEDS_B; i = i + 1)
        {
            leds_B[i] = CRGB(255, 0, 0);
        }
    }
    else
    {
        for (int i = 0; i < NUM_LEDS_B; i = i + 1)
        {
            leds_B[i] = CRGB(0, 0, 0);
        }
    }

    // Front
    if (blinker)
    {
        if (left)
        {
            for (int i = 0; i < NUM_LEDS_turn; i = i + 1)
            {
                leds_L[i] = CRGB(150, 130, 0);
                leds_R[i] = CRGB(0, 0, 0);
            }
        }
        else if (right)
        {
            for (int i = 0; i < NUM_LEDS_turn; i = i + 1)
            {
                leds_L[i] = CRGB(0, 0, 0);
                leds_R[i] = CRGB(150, 130, 0);
            }
        }
    }
    else
    {
        for (int i = 0; i < NUM_LEDS_turn; i = i + 1)
        {
            leds_L[i] = CRGB(0, 0, 0);
            leds_R[i] = CRGB(0, 0, 0);
        }
    }

    // Rear
    if (blinker)
    {
        for (int i = 0; i < NUM_LEDS_B; i = i + 1)
        {
            if ((i < NUM_TURN_B) && left)
            {
                leds_B[i] = CRGB(150, 30, 0);
            }
            else if ((i >= (NUM_LEDS_B - NUM_TURN_B)) && right)
            {
                leds_B[i] = CRGB(150, 30, 0);
            }
        }
    }

    FastLED.show();

    // Turn signals
    if (millis() < blinkerTimer + blinkerPeriod)
    {
        blinker = true;
    }
    else if (millis() < blinkerTimer + 2 * blinkerPeriod)
    {
        blinker = false;
    }
    else
    {
        blinkerTimer = millis();
    }
}
