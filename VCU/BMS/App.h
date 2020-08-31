#ifndef APP_H
#define APP_H
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

#include <BlynkSimpleEsp32_BT.h>
#include <BLEDevice.h>
#include <BLEServer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char authBT[] = "b0RT4FCAQTMxt83fbb7TeERXjTueZBiZ";

// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
// char authWifi[] = "nQeT_BJ2lbWc7p4dfe6L2oc8LfZZ_6-Q";

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "mobile-vibes";
//char pass[] = "cheddarcheese";

// char ssid[] = "G&T";
// char pass[] = "goblueforever";

 BlynkTimer timer;



//WidgetLED ledCharging(VP_charging);
//WidgetLCD lcd(VP_LCD);

void appSetup();
void appService();
void appUpdate();


//int gRPM = 0;
//int gCurrent = 0;
//int gVoltage = 0;
//int gInvTemp = 0;
//char gErr[17] = "";
//int Button1 = 0;
//int Button2 = 0;
//int slider = 0;

unsigned long tPeriod = 100;
unsigned long tStart = 0;

// App() : ledCharging(VP_charging) {};
// WidgetLED ledCharging;
// App() : lcd(VP_LCD) {};
// WidgetLCD lcd;

// WidgetLED ledCharging(VP_charging);
// WidgetLCD lcd(VP_LCD);

#endif APP_H
