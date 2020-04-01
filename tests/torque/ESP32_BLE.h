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

// Motor
#define VP_RPM 0
#define VP_I 1
#define VP_V 2
#define VP_T 3
#define VP_LCD 4  // 16x2
#define VP_B1 5   // ignition
#define VP_B2 6
#define VP_SLD 7

// Charger
#define VP_start_charge 22
#define VP_charging 25
#define VP_sld_Vmax 20
#define VP_sld_Imax 21
#define VP_val_V 23
#define VP_val_I 24
#define VP_CERR_VAC 26
#define VP_CERR_HW 27
#define VP_CERR_TEMP 28
#define VP_CERR_COM 29

// Lights
#define VP_angelRGB 30
#define VP_angel 31
#define VP_headlight 32
#define VP_brake 33
#define VP_turn 34

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

WidgetLED ledCharging(VP_charging);

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

  ledCharging.off();
//  delay(1000);
}

void BLE_writeDash()
{
  Blynk.virtualWrite(VP_RPM, CANb.getmRPM() );
  Blynk.virtualWrite(VP_I, CANb.getmCurrent() );
  Blynk.virtualWrite(VP_V, CANb.getmVoltage() );
  Blynk.virtualWrite(VP_T, CANb.getmTempI() );
//  Serial.println("SENT");

}

void BLE_writeCharge()
{
  Blynk.virtualWrite(VP_val_V, CANb.getcVoltage() / 10 );
  Blynk.virtualWrite(VP_val_I, CANb.getcCurrent() / 10 );
  if ( CANb.getcCharging() ){
    ledCharging.off();
  } else {
    ledCharging.on();
  }

  Blynk.virtualWrite(VP_CERR_VAC, CANb.getcErrorVac() );
  Blynk.virtualWrite(VP_CERR_HW, CANb.getcErrorHW() );
  Blynk.virtualWrite(VP_CERR_TEMP, CANb.getcErrorTemp() );
  Blynk.virtualWrite(VP_CERR_COM, CANb.getcErrorCom() );

  // if (i >= 400) i = 0;
  // if (n >= 6000) n = 0;
  // if (v >= 67) v = 54;
  // if (t >= 100) t = 0;

  String lcdText = "Good";
  if (CANb.getcVoltage() < 60) lcdText = "Bad ";
  lcd.print(0,0, "Voltage: ");
  lcd.print(0,1, lcdText);
}


void BLE_update()
{

  Blynk.run();
//  timer.run();

  BLE_writeDash();
  BLE_writeCharge();
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
  charger.vMax = param.asInt()*10; // assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(VP_sld_Imax)
{
  charger.iMax = param.asInt()*10; // assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(VP_start_charge)
{
  int button = param.asInt(); // assigning incoming value from pin V1 to a variable
  if ( button == 0 ) {
    Serial.println("Charging turned OFF");
    Serial.println(CANb.getcCharging());
    charger.cmdOn = false;
    CANb.sendCharger(false, charger.vMax, charger.iMax, 'Y');
  } else if ( button == 1 ) {
    Serial.println("Charging turned ON");
    Serial.println(CANb.getcCharging());
    charger.cmdOn = true;
    CANb.sendCharger(true, charger.vMax, charger.iMax, 'R');
    Serial.print("Vmax: ");
    Serial.print(charger.vMax);
    Serial.print("Imax: ");
    Serial.print(charger.iMax);
    Serial.println();
  }
  
}

// Lights

BLYNK_WRITE(VP_angel)
{
  int bin = param.asInt(); // assigning incoming value from pin V1 to a variable
  if ( bin == 0 ) {
    lights.setAngel( false );
  } else if ( bin == 1 ) {
    lights.setAngel( true );
  }
}

BLYNK_WRITE(VP_angelRGB)
{
  int rIn = param[0].asInt();
  int gIn = param[1].asInt();
  int bIn = param[2].asInt();
  lights.setAngelRGB(rIn, gIn, bIn);
}

BLYNK_WRITE(VP_brake)
{
  int bin = param.asInt(); // assigning incoming value from pin V1 to a variable
  if ( bin == 0 ) {
    lights.setBrake( false );
  } else if ( bin == 1 ) {
    lights.setBrake( true );
  }
}

BLYNK_WRITE(VP_turn)
{
  int bin = 0;
  switch (param.asInt())
  {
    case 1: {
        bin = 1;
        break;
      }
    case 2: {
        bin = 0;
         break;
      }
    case 3: {
        bin = 2;
        break;
      }
  } 
  Serial.print("Sent to setTurn: ");
  Serial.print(bin);
  Serial.println();
  lights.setTurn( bin );

}

BLYNK_WRITE(VP_headlight)
{
  int bin = param.asInt(); // assigning incoming value from pin V1 to a variable
  if ( bin == 0 ) {
    // lights.setAngel( false );
  } else if ( bin == 1 ) {
    // lights.setAngel( true );
  }
}

#endif ESP32_BLE_H
