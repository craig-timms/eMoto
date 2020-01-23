#ifndef _BATCOM_H
#define _BATCOM_H

/* 
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * 
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
 * U2UXD is unused and can be used for your projects.
 * 
*/

#define pinRXD2 16
#define pinTXD2 17
#define pinFLTn 4
#define pinWAKEn 0

class BatCOM
{
  public:    
    void setup( void );
    void restart( void );
    bool parseData( void );

  private:
    String msg = "";
    
};

#endif
