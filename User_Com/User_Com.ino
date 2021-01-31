#include <mcp2515_can.h>
#include <mcp2515_can_dfs.h>
#include <mcp2518fd_can.h>
#include <mcp2518fd_can_dfs.h>
#include <mcp_can.h>
#include <SPI.h>          

//Declare constants  
  MCP_CAN CAN(19);          //setChip Select to pin 19
  unsigned char len = 0;
  unsigned char buf[8];
  unsigned int canID;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //to communicate with Serial Monitor 
  //tries to initialize, if failed --> it will loop here for ever
START_INIT:
    if(CAN_OK == CAN.begin(CAN_500KBPS))      //setting CAN baud rate to 500Kbps (Recheck specs for CAN baud rate)
    {
        Serial.println("Initialization is ok!");
    }
    else
    {
        Serial.println("Initialization failed");
        Serial.println("Initialize attempt again");
        delay(100);
        goto START_INIT;
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  readCan(); //update this later once we are able to test  
}

void readCan() {
  
   if(CAN_MSGAVAIL == CAN.checkReceive())    //check if data is coming
    {
        CAN.readMsgBuf(&len, buf);    //read data,  len: data length, buf: data buffer
        canID = CAN.getCanId();       //getting the ID of the incoming message

        Serial.print("ID is: ");
        Serial.print(canID, HEX);     //printing the ID in its standard form, HEX

        Serial.print("    Length is: ");
        Serial.println(len);
        
        for(int i = 0; i<len; i++)    //looping on the incoming data to print them
        {
            Serial.write(buf[i]);     //Serial.write prints the character itself
        }
        Serial.println("\n\t*****************\n");
    }

   // setSOH();
   // setSOC();
}

void setSOH() {
  
} 

void setSOC() {
  
} 

void setAlarm() {
  
} 

void setChargeLED() {
  
} 
