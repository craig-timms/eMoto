#ifndef DASH_H
#define DASH_H

void BLE_writeDash()
{
  Blynk.virtualWrite(VP_RPM, CAN.getmRPM() );
  Blynk.virtualWrite(VP_I, CAN.getmCurrent() );
  Blynk.virtualWrite(VP_V, CAN.getmVoltage() );
  Blynk.virtualWrite(VP_T, CAN.getmTempI() );
//  Serial.println("SENT");

}

void BLE_writeCharge()
{
  Blynk.virtualWrite(VP_val_V, CAN.getcVoltage() );
  Blynk.virtualWrite(VP_val_I, CAN.getcCurrent() );
  Blynk.virtualWrite(VP_charging, CAN.getcCharging() );

  Blynk.virtualWrite(VP_CERR_VAC, CAN.getcErrorVac() );
  Blynk.virtualWrite(VP_CERR_HW, CAN.getcErrorHW() );
  Blynk.virtualWrite(VP_CERR_TEMP, CAN.getcErrorTemp() );
  Blynk.virtualWrite(VP_CERR_COM, CAN.getcErrorCom() );

  // if (i >= 400) i = 0;
  // if (n >= 6000) n = 0;
  // if (v >= 67) v = 54;
  // if (t >= 100) t = 0;

  String lcdText = "Good";
  if (gVoltage < 60) lcdText = "Bad ";
  lcd.print(0,0, "Voltage: ");
  lcd.print(0,1, lcdText);
}

#endif DASH_H