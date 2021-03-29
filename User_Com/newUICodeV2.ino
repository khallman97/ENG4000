
#include <CAN.h>


#define LEDR 25 // RED pin of rgb led is connected to 25 gpio pin  
 #define LEDG 26 // green pin is connected to 26 gpio  
 #define LEDB 27 // left first
 
 #define LEDR1 16 // RED pin of rgb led is connected to 25 gpio pin  
 #define LEDG1 17 // green pin is connected to 26 gpio  
 #define LEDB1 2 // left 2 second
 
// #define LEDR2 2 // RED pin of rgb led is connected to 25 gpio pin  
// #define LEDG2 0 // green pin is connected to 26 gpio  
// #define LEDB2 4 // middle 1
 
 #define LEDR3 18 // RED pin of rgb led is connected to 25 gpio pin  
 #define LEDG3 19 // green pin is connected to 26 gpio  
 #define LEDB3 21 // thrid led 
 
 #define LEDR4 12 // RED pin of rgb led is connected to 25 gpio pin  
 #define LEDG4 13 // green pin is connected to 26 gpio  
 #define LEDB4 14 // 4 led
 
 #define LEDR5 15 // RED pin of rgb led is connected to 25 gpio pin  
 #define LEDG5 22 // green pin is connected to 26 gpio  
 #define LEDB5 23 // extra alarm led
 //RGB LEFT 1   
 #define R_channel 0  
 #define G_channel 1  
 #define B_channel 2
 //RBG LEFT 2  
 #define R_channel1 3  
 #define G_channel1 4  
 #define B_channel1 5
// //RGB MIDDLE 1
// #define R_channel2 6  
// #define G_channel2 7  
// #define B_channel2 8
 //RGB MIDDLE 2
 #define R_channel3 9  
 #define G_channel3 10  
 #define B_channel3 11
 //RGB RIGHT 1
 #define R_channel4 12 
 #define G_channel4 13  
 #define B_channel4 14
 //RGB RIGHT 2
 #define R_channel5 15  
 #define G_channel5 16  
 #define B_channel5 17
 
 #define pwm_Frequency 5000 // pwm frequency  
 #define pwm_resolution 8 // 8 bit resolution  

 #define buttonPin 15
 int blue = 35; //charged
 int yellow = 33; //charging
 int red = 32; //alarm

 int SOC = 100;
 int switcher = 0;
 int SOH = 5;
 int charging = 0;
 int supplyVoltage = 0;
 
 void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  //RGB 1
  ledcAttachPin(LEDR, R_channel);  
  ledcAttachPin(LEDG, G_channel);  
  ledcAttachPin(LEDB, B_channel);   
  ledcSetup(R_channel, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel, pwm_Frequency, pwm_resolution);
  //RGB 2
  ledcAttachPin(LEDR1, R_channel1);  
  ledcAttachPin(LEDG1, G_channel1);  
  ledcAttachPin(LEDB1, B_channel1);   
  ledcSetup(R_channel1, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel1, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel1, pwm_Frequency, pwm_resolution); 
//  //RGB MIDDLE 1
//  ledcAttachPin(LEDR2, R_channel2);  
//  ledcAttachPin(LEDG2, G_channel2);  
//  ledcAttachPin(LEDB2, B_channel2);   
//  ledcSetup(R_channel2, pwm_Frequency, pwm_resolution);  
//  ledcSetup(G_channel2, pwm_Frequency, pwm_resolution);  
//  ledcSetup(B_channel2, pwm_Frequency, pwm_resolution);  
  //RGB 3
  ledcAttachPin(LEDR3, R_channel3);  
  ledcAttachPin(LEDG3, G_channel3);  
  ledcAttachPin(LEDB3, B_channel3);   
  ledcSetup(R_channel3, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel3, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel3, pwm_Frequency, pwm_resolution);  
  //RGB 4
  ledcAttachPin(LEDR4, R_channel4);  
  ledcAttachPin(LEDG4, G_channel4);  
  ledcAttachPin(LEDB4, B_channel4);   
  ledcSetup(R_channel4, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel4, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel4, pwm_Frequency, pwm_resolution);  
  //RGB Alarm
  ledcAttachPin(LEDR5, R_channel5);  
  ledcAttachPin(LEDG5, G_channel5);  
  ledcAttachPin(LEDB5, B_channel5);   
  ledcSetup(R_channel5, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel5, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel5, pwm_Frequency, pwm_resolution);     


  //Set up CAN
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
  readAndParseCAN();
  
  if(digitalRead(buttonPin)){
      if(switcher == 1){
         switcher = 0;
      } else {
        switcher = 1;
      }
  }

  buttonPress();
 
  printCharge();
  
 }

 void readAndParseCAN() {
  byte id;
  // try to parse packet
  CAN.beginExtendedPacket(0x1831f4e8);
  CAN.endPacket();
 // CAN.beginExtendedPacket(0x1831f4e8);
 
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
     // id = CAN.packetId(); //Store Id as hex
      if((CAN.packetId(), HEX) == 0x1834cce8){
         Serial.print("SOC and SOH");
      } else if((CAN.packetId(), HEX) == 0x183fcce8) {
         Serial.print("DO3");
      } else if((CAN.packetId(), HEX) == 0x183dcce8) {
        Serial.print("Alarms");
      }
//       Serial.print("packet with id 0x");
//      Serial.print(CAN.packetId(), HEX);
//      //Serial.println(" id as int " + (int)CAN.packetId());
//      if (CAN.packetRtr()) {
//        Serial.print(" and requested length ");
//        Serial.println(CAN.packetDlc());
//      } else {
//       Serial.print(" and length ");
//        Serial.println(packetSize);
//      
//      
//       // only print packet data for non-RTR packets
//      while (CAN.available()) {
//        Serial.print(CAN.read()); //parsed with (byte) before
//      }
    Serial.println();
   CAN.endPacket();
  }
 }

  void buttonPress(){
    
//    
    if (switcher == 0){
      printSOC();
    }
    else if (switcher == 1){
      printSOH();
    }
    else{
      printAlarm();
    }
  }

  void printSOC(){
    //int SOC = 100;
    if(SOC <= 10 ){
      RGB_Right2(255,255,0); // Yellow
      RGB_Right1(255,0,0); // Red
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Left1(255,0,0); // RED ccolor   
    }
    else if (SOC > 10 && SOC <= 20){
      RGB_Right2(0,255,0); // green
      RGB_Right1(255,0,0); // Red
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Left1(255,0,0); // RED ccolor   
    }
    else if (SOC > 20 && SOC <= 50){
      RGB_Right2(0,255,0); // green 
      RGB_Right1(0,255,0); // green
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Left1(255,0,0); // RED ccolor  
    }
    else if (SOC > 50 && SOC <= 80){
      RGB_Right2(0,255,0); // green 
      RGB_Right1(0,255,0); // green
      RGB_Left2(0,255,0); // green
      RGB_Left1(255,0,0); // RED ccolor  
    }
    else if (SOC > 80 && SOC <= 90){
      RGB_Right2(0,255,0); // green 
      RGB_Right1(0,255,0); // green
      RGB_Left2(0,255,0); // green
      RGB_Left1(255,255,0); // yellow
    }
    else if (SOC > 90){
      RGB_Right2(0,255,0); // green 
      RGB_Right1(0,255,0); // green
      RGB_Left2(0,255,0); // green
      RGB_Left1(0,255,0); // green
    }
  }

  void printSOH(){
    

    if (SOH <= 3){
      RGB_Right2(255,255,0); // Yellow
      RGB_Right1(255,0,0); // Red
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Left1(255,0,0); // RED ccolor   
    }
    else if (SOH > 3 && SOH <= 5){
      RGB_Right2(0,255,0); // green 
      RGB_Right1(0,255,0); // green
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Left1(255,0,0); // RED ccolor 
    }
    else if (SOH > 5 && SOH <= 7){
      RGB_Right2(0,255,0); // green 
      RGB_Right1(0,255,0); // green
      RGB_Left2(0,255,0); // green
      RGB_Left1(255,0,0); // RED ccolor  
    }
    else if (SOH > 7 && SOH <= 10){
      RGB_Right2(0,255,0); // green 
      RGB_Right1(0,255,0); // green
      RGB_Left2(0,255,0); // green
      RGB_Left1(0,255,0); // green
    }
  }

  void printCharge(){
  int batterylevel = 100;
  if(supplyVoltage == 1){
     digitalWrite(blue, HIGH); // turn the LED on by making the voltage HIGH
    if(charging == 1){
      digitalWrite(yellow, HIGH); // turn the LED off by making the voltage LOW
    }
  }
    
  
  }

  void printAlarm(){
    int alarm = 0;
    if (alarm == 0){
      //no alarm means no change in UI
      digitalWrite(red, LOW);
    }
    else{
      digitalWrite(red, HIGH);
      if (alarm == 1){
        RGB_Left1(255,255,0); // yellow ccolor
        RGB_Left2(255,255,0); // yellow ccolor
        //RGB_Middle(255,255,0); // yellow ccolor
        RGB_Right1(255,255,0); // yellow ccolor
        RGB_Right2(255,255,0); // yellow ccolor
      }
      else if (alarm == 2){
        RGB_Left1(255,0,0); // RED ccolor
        RGB_Left2(255,0,0); // RED ccolor
       // RGB_Middle(255,0,0); // RED ccolor
        RGB_Right1(255,0,0); // RED ccolor
        RGB_Right2(255,0,0); // RED ccolor
      }
      else if (alarm == 3){
        RGB_Left1(0,255,0); // green ccolor
        RGB_Left2(0,255,0); // green ccolor
        //RGB_Middle(0,255,0); // green ccolor
        RGB_Right1(0,255,0); // green ccolor
        RGB_Right2(0,255,0); // green ccolor
      }
    }
  }

  
  
 void RGB_Right2(int i, int j, int k) //left 1 
 {  
  ledcWrite(R_channel, i);   
  ledcWrite(G_channel, j);  
  ledcWrite(B_channel, k);   
 }  
 void RGB_Right1(int i, int j, int k)  //left 2
 {  
  ledcWrite(R_channel1, i);   
  ledcWrite(G_channel1, j);  
  ledcWrite(B_channel1, k);   
 } 
// void RGB_Middle(int i, int j, int k)  //middle 1
// {  
//  ledcWrite(R_channel2, i);   
//  ledcWrite(G_channel2, j);  
//  ledcWrite(B_channel2, k);   
// } 
 void RGB_Left2(int l, int m, int n)  //middle 2
 {  
  ledcWrite(R_channel3, l);   
  ledcWrite(G_channel3, m);  
  ledcWrite(B_channel3, n);   
 } 
 void RGB_Left1(int i, int j, int k)  //right 1
 {  
  ledcWrite(R_channel4, i);   
  ledcWrite(G_channel4, j);  
  ledcWrite(B_channel4, k);   
 } 
