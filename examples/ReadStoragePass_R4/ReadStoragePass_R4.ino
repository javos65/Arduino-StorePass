/***
    eeprom_put and get example using C-structure to store Login and passwords for IoT applications with Wifi and Mqtt etc.
    text data is cyphered by simple algorith, stored eeprom data is not text-readable.

    Debug.h - simple serial.print debug call
    Credentials.h - defines structure , seed and id

    Use StoragePass to store your secret credentials
    Use ReadStoragePass_R4.ino for read only functions to add to your IOT code

    Written by Jay Fox - 2024
    Released under MIT licence.
***/

#include <EEPROM.h>
#include "Debug.h"
#include "Credentials.h"

EECredentials G_test = {"MYtestnetwork","password1","admin","xxxtestxxx","192.168.100.6","admin1","testcode",0x10,1}; // Global Variable Test structure

void setup() {

  Serial.begin(115200);
  while (!Serial) { ; // wait for serial port to connect. Needed for native USB port only
  }
  delay(1000);

 Debugln("Read Test  1 - ok - RAW data"); 
 Read_Raw(&G_test);
 Debug_Credentials(&G_test);
 
 Debugln("Read Test  2 - ok - Decyphered data"); 
 Read_Credentials(&G_test);
 Debug_Credentials(&G_test);

}

void loop() {
  /* Empty loop */
}



//---------------- test function RAW read
/* Read Credentials and verify ID RAW DATA */
byte Read_Raw(struct EECredentials *O)
{
  if (Check_EEsize(O)) {    
     EECredentials t; // dummy structure to test-read
     int eeAddress = EEPROM.length()- sizeof(t)-1; // Data stored in last page of EEprom memory
     EEPROM.get(eeAddress, t);
     //Debug_Credentials(&t);
    if (t.identity == EECredentials_ID){ // check ID
        EEPROM.get(eeAddress, *O);          // real data read to Object
        Debug(" * Object read from EEprom,with ID ");Debug(t.identity);Debug(" at address 0x");Debugln(eeAddress,HEX);
        return(1);
        }
     else {
        Debugln(" * EEprom read Failed : Wrong ID");
        return(0);
        }
  }
  else { 
    Debugln(" * EEprom Read Failed : No Eeprom");
    return(0);
    }         
}






// ------------------- EEPROM CREDENTIALS READ WRITE ROUTINES
//--------------------- usable function for reading only
/* Read Credentials and verify ID */
byte Read_Credentials(struct EECredentials *O)
{
  if (Check_EEsize(O)) {    
     EECredentials t; // dummy structure to test-read
     int eeAddress = EEPROM.length()- sizeof(t)-1; // Data stored in last page of EEprom memory
     EEPROM.get(eeAddress, t);
     // Debug_Credentials(&t); // raw read
     if (t.identity == EECredentials_ID){ // check ID
        //EEPROM.get(eeAddress, *O);          // real data read to Object
        O->counter= t.counter; O->identity=t.identity;
        SimpleDecypher(t.ssid,O->ssid); SimpleDecypher(t.wifipass,O->wifipass);
        SimpleDecypher(t.login1,O->login1); SimpleDecypher(t.pass1,O->pass1);
        SimpleDecypher(t.mqttadr,O->mqttadr); SimpleDecypher(t.mqttlogin,O->mqttlogin);SimpleDecypher(t.mqttpass,O->mqttpass);
        Debug(" * Object read from EEprom,with ID ");Debug(t.identity);Debug(" at address 0x");Debugln(eeAddress,HEX);
        return(1);
        }
     else {
        Debugln(" * EEprom read Failed : Wrong ID");
        return(0);
        }
  }
  else { 
    Debugln(" * EEprom Read Failed : No Eeprom");
    return(0);
    }         
}

/* Check if eeprom size is large enough to save stucture*/
byte Check_EEsize(struct EECredentials *O)
{
  long s = sizeof(*O);
if ( EEPROM.length() > s+1 ) {
    //Debug(" * Eeprom present, size ");Debug(EEPROM.length());Debug(", object size ");Debug(s);Debugln(" bytes "); 
    return(1);
    }
else { 
    Debugln(" *No Eeprom not present.");
    return(0);
    }
}

/* Print debug information of Credentaisl structure,*/
byte Debug_Credentials(struct EECredentials *O)
{
     Debug(" * Credentials:[");Debug(O->ssid);Debug(",");Debug(O->wifipass);
     Debug("],[");Debug(O->login1);Debug(",");Debug(O->pass1);
     Debug("],[");Debug(O->mqttadr);Debug(",");Debug(O->mqttlogin);Debug(",");Debug(O->mqttpass);
     Debug("], id=");Debug(O->identity);Debug(", count=");Debugln(O->counter);
     return(1);
}


/* Simple DeCyphering the text code */
byte SimpleDecypher(char * textin, char * textout)
{
int t=0;
bool st=true;
while(textin[t]!=0) {
   textout[t]=textin[t]-EE_SEED%(111)+t%17;
   t++; if( t>=EE_MAXCHAR){Debug(" * Decypher error : data too long") ;st=false;break;}
  }
  textout[t]=0;
  if (st==true){
  //Debug(" * Decyphered ");Debug(t);Debug(" - ");Debugln(textout);
  return(1);
  }
  else return(0);
}
