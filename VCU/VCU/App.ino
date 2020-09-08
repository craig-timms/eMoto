void appSetup()
{
  // Debug console
  Serial.println("Waiting for connections...");
  Blynk.setDeviceName("eMoto");
  Blynk.begin(authBT);

  // Blynk.begin(authWifi, ssid, pass, "blynk-cloud.com", 8080);
  //    timer.setInterval(1 * 1000, reconnectBlynk);  // check every minute if still connected to server

  timer.setInterval(100L, appUpdate);

  lcd.clear();
  Blynk.virtualWrite(VP_B1,  LOW);
  Blynk.virtualWrite(VP_B2,  LOW);
  Blynk.virtualWrite(VP_RPM, 6000);
  Blynk.virtualWrite(VP_I,   400);
  Blynk.virtualWrite(VP_V,   68);
  Blynk.virtualWrite(VP_T,   100);

  ledCharging.off();
  //  delay(1000);
}

void appService()
{
  // Debug console
//  if ((millis() - tStart) > tPeriod)
//  {
//    appUpdate();
//    tStart = millis();
//    // Serial.println("App updated");
//    Serial.println(Blynk.connected());
//  }

//    if (Blynk.connected())
//    {
//      Blynk.run();
//      timer.run();
//    }
    
  Blynk.run();
  timer.run();

}

void appUpdate()
{

  appWriteDash();
  appWriteCharge();
//  Serial.println( Blynk.connected() );
}

void appWriteDash()
{
  Blynk.virtualWrite(VP_RPM, vehicle.controls.throttle);
  // Blynk.virtualWrite(VP_RPM, vehicle.mcu.RPM);
  Blynk.virtualWrite(VP_I, vehicle.mcu.current);
  Blynk.virtualWrite(VP_V, vehicle.mcu.voltage);
  Blynk.virtualWrite(VP_T, vehicle.mcu.temp);
  if ( vehicle.mcu.online ) {
    ledMCU.on();
  } else {
    ledMCU.off();
  }

//  Serial.println("SENT");
}

BLYNK_WRITE(VP_MCU_POWER)
{
  vehicle.mcu.enable = param.asInt(); // assigning incoming value from pin V1 to a variabl
  Serial.println("Enable set");
}

BLYNK_WRITE(VP_MCU_REVERSE)
{
  vehicle.mcu.reverse = param.asInt(); // assigning incoming value from pin V1 to a variabl
}

BLYNK_WRITE(VP_MCU_BRAKEAN)
{
  vehicle.mcu.brakeAN = param.asInt(); // assigning incoming value from pin V1 to a variabl
}

BLYNK_WRITE(VP_MCU_BRAKETH)
{
  vehicle.mcu.brakeThrottle = param.asInt(); // assigning incoming value from pin V1 to a variabl
}

void appWriteCharge()
{
  Blynk.virtualWrite(VP_val_V, vehicle.charger.v);
  Blynk.virtualWrite(VP_val_I, vehicle.charger.i);
  if (vehicle.charger.error_vac)
  {
    ledCharging.on();
  }
  else
  {
    ledCharging.off();
  }

  Blynk.virtualWrite(VP_CERR_VAC, vehicle.charger.error_vac);
  Blynk.virtualWrite(VP_CERR_HW, vehicle.charger.error_HW);
  Blynk.virtualWrite(VP_CERR_TEMP, vehicle.charger.error_Temp);
  Blynk.virtualWrite(VP_CERR_COM, vehicle.charger.error_Com);

  // if (i >= 400) i = 0;
  // if (n >= 6000) n = 0;
  // if (v >= 67) v = 54;
  // if (t >= 100) t = 0;

  String lcdText = "Good";
  if (vehicle.charger.v < 60)
    lcdText = "Bad ";
  lcd.print(0, 0, "Voltage: ");
  lcd.print(0, 1, lcdText);
}

BLYNK_WRITE(VP_B1)
{
  int Button1 = param.asInt(); // assigning incoming value from pin V1 to a variabl
  // process received value
  //  Serial.print("Button 1 changed to: ");
  //  Serial.print(Button1);
  //  Serial.println();
}

BLYNK_WRITE(VP_B2)
{
  int Button2 = param.asInt(); // assigning incoming value from pin V1 to a variable
  lcd.clear();
}

BLYNK_WRITE(VP_SLD)
{
  int slider = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
  Serial.print("Slider changed to: ");
  Serial.print(slider);
  Serial.println();
}

BLYNK_WRITE(VP_sld_Vmax)
{
  // charger.vMax = param.asInt() * 10; // assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(VP_sld_Imax)
{
  // charger.iMax = param.asInt() * 10; // assigning incoming value from pin V1 to a variable
}

BLYNK_WRITE(VP_start_charge)
{
  int button = param.asInt(); // assigning incoming value from pin V1 to a variable
  // if (button == 0)
  // {
  //     Serial.println("Charging turned OFF");
  //     Serial.println(CANb.getcCharging());
  //     charger.cmdOn = false;
  //     CANb.sendCharger(false, charger.vMax, charger.iMax, 'Y');
  // }
  // else if (button == 1)
  // {
  //     Serial.println("Charging turned ON");
  //     Serial.println(CANb.getcCharging());
  //     charger.cmdOn = true;
  //     CANb.sendCharger(true, charger.vMax, charger.iMax, 'R');
  //     Serial.print("Vmax: ");
  //     Serial.print(charger.vMax);
  //     Serial.print("Imax: ");
  //     Serial.print(charger.iMax);
  //     Serial.println();
  // }
}

// Lights

BLYNK_WRITE(VP_angel)
{
  int bin = param.asInt(); // assigning incoming value from pin V1 to a variable
  // if (bin == 0)
  // {
  //     lights.setAngel(false);
  // }
  // else if (bin == 1)
  // {
  //     lights.setAngel(true);
  // }
}

BLYNK_WRITE(VP_angelRGB)
{
  int rIn = param[0].asInt();
  int gIn = param[1].asInt();
  int bIn = param[2].asInt();
  // lights.setAngelRGB(rIn, gIn, bIn);
}

BLYNK_WRITE(VP_brake)
{
  int bin = param.asInt(); // assigning incoming value from pin V1 to a variable
  // if (bin == 0)
  // {
  //     lights.setBrake(false);
  // }
  // else if (bin == 1)
  // {
  //     lights.setBrake(true);
  // }
}

BLYNK_WRITE(VP_turn)
{
  int bin = 0;
  switch (param.asInt())
  {
    case 1:
      {
        bin = 1;
        break;
      }
    case 2:
      {
        bin = 0;
        break;
      }
    case 3:
      {
        bin = 2;
        break;
      }
  }
  Serial.print("Sent to setTurn: ");
  Serial.print(bin);
  Serial.println();
  // lights.setTurn(bin);
}

BLYNK_WRITE(VP_headlight)
{
  int bin = param.asInt(); // assigning incoming value from pin V1 to a variable
  if (bin == 0)
  {
    // lights.setAngel( false );
  }
  else if (bin == 1)
  {
    // lights.setAngel( true );
  }
}
