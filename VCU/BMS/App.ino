#include "App.h"

WidgetLED onlineLED(VP_CHRG_ONLINE);
WidgetLED eLED1(VP_CHRG_ETEMP);
WidgetLED eLED2(VP_CHRG_EVAC);
WidgetLED eLED3(VP_CHRG_EHW);
WidgetLED eLED4(VP_CHRG_ECOM);

void appSetup()
{
  // Debug console
  Serial.println("Waiting for connections...");
   Blynk.setDeviceName("eMoto-BMS");
   Blynk.begin(authBT);
  // Blynk.begin(authWifi, ssid, pass, "blynk-cloud.com", 8080);
  //    timer.setInterval(1 * 1000, reconnectBlynk);  // check every minute if still connected to server
  timer.setInterval(100L, appUpdate);
  Blynk.virtualWrite(VP_CHRG_I, vehicle.charger.iMax);
  Blynk.virtualWrite(VP_CHRG_V, vehicle.charger.vMax);
  onlineLED.off();
  eLED1.off();
  eLED2.off();
  eLED3.off();
  eLED4.off();
//  lcd.clear();
//  Blynk.virtualWrite(VP_B1,  LOW);
//  Blynk.virtualWrite(VP_B2,  LOW);
//  Blynk.virtualWrite(VP_RPM, 6000);
//  Blynk.virtualWrite(VP_I,   400);
//  Blynk.virtualWrite(VP_V,   68);
//  Blynk.virtualWrite(VP_T,   100);

//  ledCharging.off();
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
  Blynk.virtualWrite(VP_VLEVEL, vehicle.battery.voltage/100);
  Blynk.virtualWrite(VP_VDIGITAL, vehicle.battery.voltage*10);
  Blynk.virtualWrite(VP_CHRG_RV, vehicle.charger.rV);
  Blynk.virtualWrite(VP_CHRG_RI, vehicle.charger.rI);

  // Charger
  if ( vehicle.charger.online ) {
    onlineLED.on();
  } else {
    onlineLED.off();
  }
  
  if ( vehicle.charger.eTemp ) {
    eLED1.on();
  } else {
    eLED1.off();
  }
  
  if ( vehicle.charger.eVac ) {
    eLED2.on();
  } else {
    eLED2.off();
  }
  
  if ( vehicle.charger.eHW ) {
    eLED3.on();
  } else {
    eLED3.off();
  }
  
  if ( vehicle.charger.eCom ) {
    eLED4.on();
  } else {
    eLED4.off();
  }
}

// void appWriteDash()
// {
//   Blynk.virtualWrite(VP_RPM, vehicle.controls.throttle);
//   // Blynk.virtualWrite(VP_RPM, vehicle.mcu.RPM);
//   Blynk.virtualWrite(VP_I, vehicle.mcu.current);
//   Blynk.virtualWrite(VP_V, vehicle.mcu.voltage);
//   Blynk.virtualWrite(VP_T, vehicle.mcu.temp);
//   Serial.println("SENT");
// }

// void appWriteCharge()
// {
//   Blynk.virtualWrite(VP_val_V, vehicle.charger.v);
//   Blynk.virtualWrite(VP_val_I, vehicle.charger.i);
//   if (vehicle.charger.error_vac)
//   {
//     ledCharging.on();
//   }
//   else
//   {
//     ledCharging.off();
//   }

//   Blynk.virtualWrite(VP_CERR_VAC, vehicle.battery.voltage);
//   Blynk.virtualWrite(VP_CERR_HW, vehicle.charger.error_HW);
//   Blynk.virtualWrite(VP_CERR_TEMP, vehicle.charger.error_Temp);
//   Blynk.virtualWrite(VP_CERR_COM, vehicle.charger.error_Com);

//   // if (i >= 400) i = 0;
//   // if (n >= 6000) n = 0;
//   // if (v >= 67) v = 54;
//   // if (t >= 100) t = 0;

//   String lcdText = "Good";
//   if (vehicle.charger.v < 60)
//     lcdText = "Bad ";
//   lcd.print(0, 0, "Voltage: ");
//   lcd.print(0, 1, lcdText);
// }

BLYNK_WRITE(VP_PRE)
{
  int Button1 = param.asInt();
  if ( Button1 == 1 ) { 
    vehicle.battery.appPre = true;
  } else
  {
    vehicle.battery.appPre = false;
  }
}

BLYNK_WRITE(VP_BLEED)
{
  int Button1 = param.asInt();
  if ( Button1 == 1 ) { 
    vehicle.battery.appBleed = true;
  } else
  {
    vehicle.battery.appBleed = false;
  }
}

BLYNK_WRITE(VP_HV)
{
  int Button1 = param.asInt();
  if ( Button1 == 1 ) { 
    vehicle.battery.appHV = true;
  } else
  {
    vehicle.battery.appHV = false;
  }
}

BLYNK_WRITE(VP_CHRG_EN)
{
  int Button1 = param.asInt();
  if ( Button1 == 1 ) { 
    vehicle.charger.enable = true;
  } else
  {
    vehicle.charger.enable = false;
  }
}

BLYNK_WRITE(VP_CHRG_I)
{
  int slider = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
  vehicle.charger.iMax = slider;
}

BLYNK_WRITE(VP_CHRG_V)
{
  int slider = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
  vehicle.charger.vMax = slider;
}

// BLYNK_WRITE(VP_SLD)
// {
//   int slider = param.asInt(); // assigning incoming value from pin V1 to a variable

//   // process received value
//   Serial.print("Slider changed to: ");
//   Serial.print(slider);
//   Serial.println();
// }

// BLYNK_WRITE(VP_sld_Vmax)
// {
//   // charger.vMax = param.asInt() * 10; // assigning incoming value from pin V1 to a variable
// }

// BLYNK_WRITE(VP_sld_Imax)
// {
//   // charger.iMax = param.asInt() * 10; // assigning incoming value from pin V1 to a variable
// }

// BLYNK_WRITE(VP_start_charge)
// {
//   int button = param.asInt(); // assigning incoming value from pin V1 to a variable
// }


// BLYNK_WRITE(VP_angelRGB)
// {
//   int rIn = param[0].asInt();
//   int gIn = param[1].asInt();
//   int bIn = param[2].asInt();
//   // lights.setAngelRGB(rIn, gIn, bIn);
// }


// BLYNK_WRITE(VP_turn)
// {
//   int bin = 0;
//   switch (param.asInt())
//   {
//     case 1:
//       {
//         bin = 1;
//         break;
//       }
//     case 2:
//       {
//         bin = 0;
//         break;
//       }
//     case 3:
//       {
//         bin = 2;
//         break;
//       }
//   }
//   Serial.print("Sent to setTurn: ");
//   Serial.print(bin);
//   Serial.println();
//   // lights.setTurn(bin);
// }

// BLYNK_WRITE(VP_headlight)
// {
//   int bin = param.asInt(); // assigning incoming value from pin V1 to a variable
//   if (bin == 0)
//   {
//     // lights.setAngel( false );
//   }
//   else if (bin == 1)
//   {
//     // lights.setAngel( true );
//   }
// }
