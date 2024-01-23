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

////////// THIS IS YOUR SECRET  !!! //////
EECredentials G_Object = {"LO C!ALnetwork~XYZ5000","NothingD!~!House","admin!X","AM39!@f010!QSSAM23~F05","www.google.com/mqttserver23","admin1","pass!ode plusext~~a1234",0x13,1}; // Global Variable Object structure
////////// THIS IS YOUR SECRET  !!! //////

EECredentials G_test = {"MYtestnetwork","password1","Testadmin","xxxtestxxx","192.168.100.63","admin1","testcode",0x10,1}; // Global Variable Test structure

void setup() {
/*************SERIAL SETUP ***************/
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  delay(1000);
/*************EEPROM SETUP ***************/  
  int eesize = EEPROM.length();
  int crsize = sizeof(G_Object);
  Debug(" * Found EEprom, length: ");Debugln( eesize);
  Debug(" * Credential struct, length: ");Debugln(crsize);
  if( (crsize+1) < eesize ) {   Debug(" * EEprom write-address at: 0x");Debugln( eesize-crsize-1,HEX);}
  else {
    Debugln(" * Size does not fit ");
    while(1);}

  
 Debugln("\nSave Object  1 - ok"); 
 Debug_Credentials(&G_Object);
 Save_Credentials(&G_Object); // should be ok
 
 Debugln("Save test  2 - fail"); 
 Debug_Credentials(&G_test);
 Save_Credentials(&G_test); // should fail

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

 Debug("\nYour Wifi Credentials [SSID],[Pass]=[");Debug(G_Object.ssid);Debug("],[");Debug(G_Object.wifipass);Debugln("]");
}



void loop() {
  /* Empty loop */
}





// ------------------- EEPROM CREDENTIALS READ WRITE ROUTINES

/* verify ID, verify EEprom, Safe data, increase counter*/
byte Save_Credentials(struct EECredentials *O )
{
  //Debug_Credentials(O);
  if (Check_EEsize(O)) {
     long c = O->counter+1;
     EECredentials t; // dummy structure to test-read
     int eeAddress = EEPROM.length()- sizeof(t)-1; // Data stored in last page of EEprom memory
     EEPROM.get(eeAddress, t);
     if(t.counter > 0) c = t.counter +1; // if valid counter in Eeprom, use it and increase
     if (O->identity == EECredentials_ID) {
         O->counter=c; t.counter = c; t.identity=EECredentials_ID;
         SimpleCypher(O->ssid,t.ssid); SimpleCypher(O->wifipass,t.wifipass);
         SimpleCypher(O->login1,t.login1); SimpleCypher(O->pass1,t.pass1);
         SimpleCypher(O->mqttadr,t.mqttadr); SimpleCypher(O->mqttlogin,t.mqttlogin);SimpleCypher(O->mqttpass,t.mqttpass);
       EEPROM.put(eeAddress, t);
       Debug(" * Saved Object to EEprom with ID ");Debug(EECredentials_ID);Debug(" at address 0x");Debugln(eeAddress,HEX);
       return(1);
       }
     else  {
       Debugln(" * Object not saved : wrong ID");
       return(0);      
       }
    } // end check size
  else  {
   Debugln(" * Object not saved : no EEprom memory");
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
    //Debugln(" *No Eeprom not present.");
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

/* Erase Object in EEprom*/
byte Erase_Credentials(struct EECredentials *O )
{
  if (Check_EEsize(O)) {  
    if (O->identity == EECredentials_ID) {
      int eeAddress = EEPROM.length()- sizeof(*O)-1; // Data stored in last page of EEprom memory
      struct EECredentials x;
      x.identity = 0;
      EEPROM.put(eeAddress, x);
      Debugln(" * Object erased from EEprom");
      return(1);
    }
    else  {
      Debugln(" * Erase Failed: wrong ID");
      return(0);      
      } 
  } 
  else { 
    //Debugln(" *Erase Failed : No Eeprom");
    return(0);
    }     
}

/* Read Credentials and verify ID */
byte Read_Credentials(struct EECredentials *O)
{
  if (Check_EEsize(O)) {    
     EECredentials t; // dummy structure to test-read
     int eeAddress = EEPROM.length()- sizeof(t)-1; // Data stored in last page of EEprom memory
     EEPROM.get(eeAddress, t);
     Debug_Credentials(&t); // raw read
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


/* Simple Cyphering the text code */
byte SimpleCypher(char * textin, char * textout)
{
int t=0;
bool st=true;
while(textin[t]!=0) {
   textout[t]=textin[t]+EE_SEED%(111)-t%17;
   t++; if( t>=EE_MAXCHAR){Debug(" * Cypher error: data size out of range") ;st=false;break;}
  }
  textout[t]=0;
  if (st==true){
  //Debug(" * Decyphered ");Debug(t);Debug(" - ");Debugln(textout);
  return(1);
  }
  else return(0);
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
