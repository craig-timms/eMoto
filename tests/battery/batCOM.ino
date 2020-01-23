# include "batCOM.h"

String tempMsg = "";
bool stringDone = false;
int byteCount = 1;

void
BatCOM::setup( void )
{
  pinMode(pinFLTn, INPUT);
  pinMode(pinWAKEn, OUTPUT);

  digitalWrite(pinWAKEn, LOW);
  delay(1000);
  digitalWrite(pinWAKEn, HIGH);
  delay(1000);
  digitalWrite(pinWAKEn, LOW);
  Serial.println("Wake Cycled");

  msg.reserve(200);
  restart();
  
}

void
BatCOM::restart( void )
{
  if (Serial2 )
  {
    Serial2.end();
  }
  byteCount = 1;
  Serial2.begin(250000, SERIAL_8N1, pinRXD2, pinTXD2);
  Serial.println("Serial Restarted");
//  Serial.println("Serial Txd is on pin: "+String(TXD2));
//  Serial.println("Serial Rxd is on pin: "+String(RXD2));
}

bool
BatCOM::parseData( void )
{
  if (stringDone) {
    Serial.println(msg);
    // clear string:
    msg = tempMsg;
    tempMsg = "";
    byteCount = 1;
    stringDone = false;
  }
}

void serialEvent2()
{
  while (Serial2.available()) {
    // get new byte:
    char inChar = (char)Serial.read();
    // add it to the msg:
    tempMsg += inChar;
    // figure out when done
    // add CRC
    if (byteCount > 15) {
      stringDone = true;
    }
  }
}
