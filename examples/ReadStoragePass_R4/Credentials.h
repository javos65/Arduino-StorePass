// ------------------- EEPROM CREDENTIALS FOR READ WRITE ROUTINES
//    SEE GITHUB https://github.com/javos65/Arduino-StorePass
//   
#ifndef __CRED_H
#define __CRED_H

#include <Arduino.h>
#define byte uint8_t
#define EECredentials_ID 0x55   // EEprom save ID for object to retrieve
#define EE_SEED 71              // Seed for Cyphering
#define EE_MAXCHAR 32           // structure for EEprom storage:  7 x 32byte + 5 bytes = 229 bytes length + overhead = 232 bytes in total

struct EECredentials {          
char ssid[EE_MAXCHAR];
char wifipass[EE_MAXCHAR];
char login1[EE_MAXCHAR];
char pass1[EE_MAXCHAR];
char mqttadr[EE_MAXCHAR];
char mqttlogin[EE_MAXCHAR];
char mqttpass[EE_MAXCHAR];
byte identity;
long counter;
};

// Declare external functions in Credentials.cpp that cal be called from the sketch //
extern byte Debug_Credentials(struct EECredentials *O);
extern byte Read_Credentials(struct EECredentials *O);
extern int Check_EEsize();
extern byte SimpleDecypher(char * textin, char * textout);
extern byte SimpleCypher(char * textin, char * textout);

#endif
