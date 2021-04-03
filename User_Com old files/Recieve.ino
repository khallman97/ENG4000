// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


#include <CAN.h>


void setup() {
  Serial.begin(9600);
  //while (!Serial);
  CAN.setPins(4, 5);
  Serial.println("CAN Receiver");
  // start the CAN bus at 500 kbps
  if (!CAN.begin(250E3)) {
  Serial.println("Starting CAN failed!");
  while (1);
  }
  
  CAN.endPacket();
  
}


void loop() {
  // try to parse packet
  CAN.beginExtendedPacket(0x1831f4e8);
//  CAN.endPacket();
  CAN.beginExtendedPacket(0x1831f4e8);
 
  //delay(500);
  int packetSize = CAN.parsePacket();
  
  //CAN.sendMsgBuf(msg ID, extended?, #of data bytes, data array);
  
  if (packetSize) {
  // received a packet
  Serial.print("Received ");
  
  
   if (CAN.packetExtended()) {
    Serial.print("extended ");
    }
    
    
     if (CAN.packetRtr()) {
    // Remote transmission request, packet contains no data
      Serial.print("RTR ");
      }
      
      
       Serial.print("packet with id 0x");
      Serial.print(CAN.packetId(), HEX);
      //Serial.println(" id as int " + (int)CAN.packetId());
      if (CAN.packetRtr()) {
        Serial.print(" and requested length ");
        Serial.println(CAN.packetDlc());
      } else {
       Serial.print(" and length ");
        Serial.println(packetSize);
      
      
       // only print packet data for non-RTR packets
      while (CAN.available()) {
        Serial.print(CAN.read()); //parsed with (byte) before
      }
    Serial.println();
  }
  
  
   Serial.println();
   CAN.endPacket();
  }
  
}
