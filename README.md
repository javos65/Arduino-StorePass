= StorePass for Arduino =

Addon code to store your credentials for IoT cyphered in EEprom

#StoragePass_R4.ino:
setup your credential structure with your secrets, store them in EEprom.
Cyphering is very simple, but readout the eeprom wont show any readable details
Set the Cypher Seed(int) and your Structure ID(byte) in credentials.h

#ReadStoragePass_R4.ino:
Read funtions only for reading your credentials back, to integrate in your own code.
Set the Cypher Seed(int) and your Structure ID(byte) in credentials.h

Code build and tested on Uno R4 with 8Kbyte Eeprom, but should work on any board that supports the EEPROM.h library


For more information about this library please visit us at
http://www.github

== License ==

Copyright (c) Arduino LLC. All right reserved.
Copyright (c) Waveshare team

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

# Arduino-StorePass
 Store Password Credentials cyphered in EEprom
