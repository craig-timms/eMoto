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
#define VP_B1 5   // ignition
#define VP_B2 6
#define VP_SLD 7

#define VP_start_charge 22
#define VP_charging 25
#define VP_sld_Vmax 20
#define VP_sld_Imax 21
#define VP_val_V 23
#define VP_val_I 24
#define VP_CERR_VAC 26
#define VP_CERR_HW 27
#define VP_CERR_TEMP 28
#define VP_CERR_COM 

unsigned long BLEtime = 0;
//unsigned long BLEtime_diff = 0;

BlynkTimer timer;
WidgetLCD lcd(VP_LCD);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "CnlLxWoZB2RbuKdGGiEIObOokHatLA2T";

int gRPM = 0;
int gCurrent = 0;
int gVoltage = 0;
int gInvTemp = 0;
char gErr[17] = "";
int Button1 = 0;
int Button2 = 0;
int slider = 0;

#include "app_charge.h"
#include "app_dash.h"

void BLE_setup()
{
  // Debug console
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("eMoto");
  Blynk.begin(auth);
//  timer.setInterval(100L, BLE_writeGauges);
  lcd.clear();
  Blynk.virtualWrite(VP_B1, LOW);
  Blynk.virtualWrite(VP_B2, LOW);
  Blynk.virtualWrite(VP_RPM, 6000);
  Blynk.virtualWrite(VP_I, 400);
  Blynk.virtualWrite(VP_V, 68);
  Blynk.virtualWrite(VP_T, 100);
//  delay(1000);
}

void BLE_update()
{

  Blynk.run();
//  timer.run();

  BLE_writeDash();
}

BLYNK_WRITE(VP_B1)
{
  Button1 = param.asInt(); // assigning incoming value from pin V1 to a variable
  if ( Button1 == 0 ) {
    Serial.println("Turn-off initiated");
    motor.setHV( 3 );
  } else if ( Button1 == 1 ) {
    Serial.println("Turn-on initiated");
    motor.setHV( 1 );
  }

  // process received value
//  Serial.print("Button 1 changed to: ");
//  Serial.print(Button1);
//  Serial.println();
}

BLYNK_WRITE(VP_B2)
{
  Button2 = param.asInt(); // assigning incoming value from pin V1 to a variable
  lcd.clear();
}

BLYNK_WRITE(VP_SLD)
{
  slider = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
  Serial.print("Slider changed to: ");
  Serial.print(slider);
  Serial.println();
}

BLYNK_WRITE(VP_sld_Vmax)
{
  charger.vMax = param.asInt(); // assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(VP_sld_Imax)
{
  charger.iMax = param.asInt(); // assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(VP_start_charge)
{
  int button = param.asInt(); // assigning incoming value from pin V1 to a variable
  if ( button == 0 ) {
    Serial.println("Charging turned OFF");
    CAN.sendCharger(false, charger.vMax, charger.iMax, 'Y');
  } else if ( Button1 == 1 ) {
    Serial.println("Charging turned ON");
    CAN.sendCharger(true, charger.vMax, charger.iMax, 'R');
  }
  
}


#endif ESP32_BLE_H
