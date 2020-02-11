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

int i = 0;
int n = 0;
int v = 53;
int t = 0;

void writeGauges()
{
//  static int value = 0;
//  static int delta = 30;
//  value += delta;
//  if (value > 255 || value < 0) {
//    delta = -delta;
//  } else {
//    Serial.print("LED on V2: ");
//    Serial.println(value);
//    led2.setValue(value);
//  }
  
  Blynk.virtualWrite(VP_RPM, n+=200);
  Blynk.virtualWrite(VP_I, i+=10);
  Blynk.virtualWrite(VP_V, v++);
  Blynk.virtualWrite(VP_T, t+=10);
//  Serial.println("SENT");

  if (i >= 400) i = 0;
  if (n >= 6000) n = 0;
  if (v >= 67) v = 54;
  if (t >= 100) t = 0;

  String lcdText = "Good";
  if (v < 60) lcdText = "Bad ";
  lcd.print(0,0, "Voltage: ");
  lcd.print(0,1, lcdText);
}

BLYNK_WRITE(VP_B1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
  Serial.print("Button 1 changed to: ");
  Serial.print(pinValue);
  Serial.println();
}

BLYNK_WRITE(VP_B2)
{
  lcd.clear();
}

BLYNK_WRITE(VP_SLD)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
  Serial.print("Slider changed to: ");
  Serial.print(pinValue);
  Serial.println();
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Serial.println("Waiting for connections...");

  Blynk.setDeviceName("Blynk");

  Blynk.begin(auth);

  timer.setInterval(100L, writeGauges);
  lcd.clear();
}

void loop()
{
  Blynk.run();

  timer.run();

//  lcd.clear();
//  lcd.print(0,0,"Errors");
}
