/* 
 * Craig Timms
 * 
 * 
 * 
*/


//HardwareSerial batUART(1); // uart 1
//HardwareSerial tempUART(2); // uart 2

//int msg[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int msg[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
String strMsg = "Hello";
 
void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(115200);

}
 
void loop() { //Choose Serial1 or Serial2 as required
//  int sec = millis() / 1000;
//  int min = sec / 60;
//  int hr = min / 60;
//  char array1[3] = {char(hr), char(min), char(sec)};
  delay(5000);

//  Serial.print(int(array1[0]));
//  Serial.print(":");
//  Serial.print(int(array1[1]));
//  Serial.print(":");
//  Serial.print(int(array1[2]));
//  Serial.println();
   
//  Serial2.write(msg,sizeof(msg));
//  tempUART.write(msg,sizeof(msg));
   
//  Serial.print( msg[3], HEX );
//  Serial.println( "Fault: "+String(digitalRead(pinFLTn)) );  
    
  if (Serial2.available() > 0) { 
    int availableBytes = Serial2.available();
    Serial.println( availableBytes, DEC );
    msg[0] = Serial2.read();
//    msg[1] = Serial2.read();
//    msg[2] = Serial2.read();
//    msg[3] = Serial2.read();
//    msg[4] = Serial2.read();
//    msg[5] = Serial2.read();
//    msg[6] = Serial2.read();
//    msg[7] = Serial2.read();
//    msg[8] = Serial2.read();
//    msg[9] = Serial2.read();
//    
//    Serial.print( "0: " );
//    Serial.println( msg[0], HEX );
//    Serial.print( "1: " );
//    Serial.println( msg[1], HEX );
//    Serial.print( "2: " );
//    Serial.println( msg[2], HEX );
//    Serial.print( "3: " );
//    Serial.println( msg[3], HEX );
//    Serial.print( "4: " );
//    Serial.println( msg[4], HEX );
//    Serial.print( "5: " );
//    Serial.println( msg[5], HEX );
//    Serial.print( "6: " );
//    Serial.println( msg[6], HEX );
//    Serial.print( "7: " );
//    Serial.println( msg[7], HEX );
//    Serial.print( "8: " );
//    Serial.println( msg[8], HEX );
//    Serial.print( "9: " );
//    Serial.println( msg[9], HEX );
//    Serial.println();
    
//    Serial.println( Serial2.available() );
//    Serial.println();
//    Serial.print(int(array1[0]));
//    Serial.print(":");
//    Serial.print(int(array1[1]));
//    Serial.print(":");
//    Serial.print(int(array1[2]));
//    Serial.println();
//    Serial.println("Sent: "+String(array1[0])+":"+String(array1[1])+":"+String(array1[2]));
//    Serial.print(char(Serial2.read()));
  } else {
      Serial2.begin(250000, SERIAL_8N1, pinRXD2, pinTXD2);
      Serial.println("Serial Attempted");
  }
}
