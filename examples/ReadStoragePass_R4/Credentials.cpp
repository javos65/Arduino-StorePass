// ------------------- EEPROM CREDENTIALS FOR READ WRITE ROUTINES
//    SEE GITHUB https://github.com/javos65/Arduino-StorePass
//
#include "Debug.h"
#include "Credentials.h"
#include <EEPROM.h>


// ------------------- EEPROM CREDENTIALS READ WRITE ROUTINES
  

/* Check if eeprom size is large enough to save stucture*/
int Check_EEsize()
{
  int crsize = sizeof(EECredentials);
  int eesize = EEPROM.length();
  if ( eesize > crsize+1 ) { return(eesize-crsize-1); } // return start adress
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

/* Read Credentials and verify ID */
byte Read_Credentials(struct EECredentials *O)
{
  if (int eeAddress = Check_EEsize()) {    
     EECredentials t; // dummy structure to test-read
     EEPROM.get(eeAddress, t);
     Debug_Credentials(&t); // raw read
     if (t.identity == EECredentials_ID){ // check ID
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
