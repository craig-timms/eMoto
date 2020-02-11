#include "ESP32_BLE.h"

void
ESP32_BLE::setup()
{
  // Debug console
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("Blynk");
  Blynk.begin(auth);
  timer.setInterval(100L, writeGauges);
  lcd.clear();
}

void 
ESP32_BLE::update( int tRPM, int tCurrent, int tVoltage, int tItemp );
{
  RPM = tRPM;
  current = tCurrent;
  voltage = tVoltage;
  invTemp = tItemp;

  Blynk.run();
  timer.run();
}

void 
ESP32_BLE::writeGauges()
{
  Blynk.virtualWrite(VP_RPM, RPM);
  Blynk.virtualWrite(VP_I, current);
  Blynk.virtualWrite(VP_V, voltage);
  Blynk.virtualWrite(VP_T, invTemp);
//  Serial.println("SENT");

  // if (i >= 400) i = 0;
  // if (n >= 6000) n = 0;
  // if (v >= 67) v = 54;
  // if (t >= 100) t = 0;

  String lcdText = "Good";
  if (v < 60) lcdText = "Bad ";
  lcd.print(0,0, "Voltage: ");
  lcd.print(0,1, lcdText);
}

ESP32_BLE::BLYNK_WRITE(VP_B1)
{
  B1 = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
  Serial.print("Button 1 changed to: ");
  Serial.print(B1);
  Serial.println();
}

ESP32_BLE::BLYNK_WRITE(VP_B2)
{
  B2 = param.asInt(); // assigning incoming value from pin V1 to a variable
  lcd.clear();
}

ESP32_BLE::BLYNK_WRITE(VP_SLD)
{
  slider = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
  Serial.print("Slider changed to: ");
  Serial.print(slider);
  Serial.println();
}


