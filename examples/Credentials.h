// ------------------- EEPROM CREDENTIALS FOR READ WRITE ROUTINES
#define EECredentials_ID 0x13   // EEprom save ID for object to retrieve
#define EE_SEED 55              // Seed for Cyphering
#define EE_MAXCHAR 32           //

struct EECredentials {          // structure for EEprom storage:  7 x 32byte + 5 bytes = 229 bytes length + overhead = 232 bytes in total
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
