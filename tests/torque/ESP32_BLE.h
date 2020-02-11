#ifndef ESP32_BLE_H
#define ESP32_BLE_H
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This example shows how to use ESP32 BLE
  to connect your project to Blynk.
  Warning: Bluetooth support is in beta!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define BLYNK_USE_DIRECT_CONNECT

// #include <BlynkSimpleEsp32_BLE.h>
#include <BlynkSimpleEsp32_BT.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#define VP_RPM 0
#define VP_I 1
#define VP_V 2
#define VP_T 3
#define VP_LCD 4  // 16x2
#define VP_B1 5
#define VP_B2 6
#define VP_SLD 7

BlynkTimer timer;
WidgetLCD lcd(VP_LCD);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "CnlLxWoZB2RbuKdGGiEIObOokHatLA2T";

class ESP32_BLE
{
    public:
        void setup( void );        
        void writeGauges( void );     
        void update( int tRPM, int tCurrent, int tVoltage, int tItemp );

    private:
      int RPM = 0;
      int current = 0;
      int voltage = 0;
      int invTemp = 0;
      char[17] = "";

      int B1 = 0;
      int B2 = 0;
      int slider = 0;
};

#endif ESP32_BLE_H