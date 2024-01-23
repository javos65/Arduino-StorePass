/***
    eeprom_put and get example using C-structure to store Login and passwords for IoT applications with Wifi and Mqtt etc.
    text data is cyphered by simple algorith, stored eeprom data is not text-readable.

    SEE GITHUB https://github.com/javos65/Arduino-StorePass

    Debug.h - simple serial.print debug call
    Credentials.h - defines structure , seed and id

    Use StoragePass to store your secret credentials
    Use ReadStoragePass_R4.ino for read only functions to add to your IOT code

    Written by Jay Fox - 2024
    Released under MIT licence.
***/

#include "Debug.h"
#include "Credentials.h"

////////// THIS IS YOUR SECRET  !!! //////
EECredentials G_Object = {"JVerfgESS","FosdgHouse","Foxadmin","AMafsSS","192.168.200.20","admin1","pass1",0x77,10,0}; // Global Variable Object structure
////////// THIS IS YOUR SECRET  !!! //////

EECredentials G_test = {"MYtestnetwork","password1","Testadmin","xxxtestxxx","192.168.100.63","admin1","testcode",0x10,1,0}; // Global Variable Test structure

void setup() {
/*************SERIAL SETUP ***************/
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  delay(1000);
/*************EEPROM SETUP ***************/  
if (long st = Check_EEsize() ){
    Debug(" * EEprom fits, write-address at: 0x");Debugln( st,HEX);
    }
else {
    Debugln(" * Size does not fit ");
    while(1);}
    
 //Erase_Credentials(&G_Object);
 
 Debugln("\nSave Object  1 - ok"); 
 Debug_Credentials(&G_Object);
 Save_Credentials(&G_Object); // should be ok
 
 Debugln("Save test  2 - fail"); 
 Debug_Credentials(&G_test);
 Save_Credentials(&G_test); // should fail
 Debug_Credentials(&G_test);

 Debugln("Read Test  3 - ok"); 
 Read_Credentials(&G_test);
 Debug_Credentials(&G_test);

 Debugln("Erase & read  4 - ok and fail read"); 
 Erase_Credentials(&G_test);
 Read_Credentials(&G_test);
 Debug_Credentials(&G_test);

 Debugln("Save Object  5 - ok"); 
 Debug_Credentials(&G_Object);
 Save_Credentials(&G_Object); // should be ok
 Read_Credentials(&G_test);
 Debug_Credentials(&G_test);

 Debugln("Save Object  5 - ok"); 
 Debug_Credentials(&G_Object);
 Save_Credentials(&G_Object); // should be ok, increased
 Read_Credentials(&G_test);
 Debug_Credentials(&G_test);
 Serial.print("\nYour Wifi Credentials [SSID],[Pass]=[");Serial.print(G_Object.ssid);Serial.print("],[");Serial.print(G_Object.wifipass);Serial.println("]");

 // Debug_Credentials(&G_Object);
 //Save_Credentials(&G_Object); // should be ok, increased
 
}



void loop() {
  /* Empty loop */
}
