#ifndef PLOT_H
#define PLOT_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>

class Browser
{
    public:
        void setup( void );
        
    private:

        AsyncWebServer server(80);
        const char* ssid = ""; // Your WiFi SSID
        const char* password = ""; // Your WiFi Password
}

//SSID and Password of your WiFi router
const char* ssid = "your-wifi";
const char* password = "your-password";
 
//ESP8266WebServer server(80); //Server on port 80
WiFiServer server(80); //Server on port 80

#endif PLOT_H