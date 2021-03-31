 #define LEDR 25 // RED pin of rgb led is connected to 25 gpio pin  
 #define LEDG 26 // green pin is connected to 26 gpio  
 #define LEDB 27 // right 1
 
 #define LEDR1 16 // RED pin of rgb led is connected to 25 gpio pin  
 #define LEDG1 17 // green pin is connected to 26 gpio  
 #define LEDB1 5 // right 2
 
 #define LEDR2 2 // RED pin of rgb led is connected to 25 gpio pin  
 #define LEDG2 0 // green pin is connected to 26 gpio  
 #define LEDB2 4 // middle 1
 
 #define LEDR3 18 // RED pin of rgb led is connected to 25 gpio pin  
 #define LEDG3 19 // green pin is connected to 26 gpio  
 #define LEDB3 21 // middle 2
 
 #define LEDR4 12 // RED pin of rgb led is connected to 25 gpio pin  
 #define LEDG4 13 // green pin is connected to 26 gpio  
 #define LEDB4 14 // left 1

 //RGB LEFT 1   
 #define R_channel 0  
 #define G_channel 1  
 #define B_channel 2
 //RBG LEFT 2  
 #define R_channel1 3  
 #define G_channel1 4  
 #define B_channel1 5
 //RGB MIDDLE 1
 #define R_channel2 6  
 #define G_channel2 7  
 #define B_channel2 8
 //RGB MIDDLE 2
 #define R_channel3 9  
 #define G_channel3 10  
 #define B_channel3 11
 //RGB RIGHT 1
 #define R_channel4 12 
 #define G_channel4 13  
 #define B_channel4 14
 
 #define pwm_Frequency 5000 // pwm frequency  
 #define pwm_resolution 8 // 8 bit resolution  

 int blue = 23; //charged
 int yellow = 22; //charging
 int red = 32; //alarm
 
 void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(yellow, OUTPUT);

  digitalWrite(red, HIGH);
  digitalWrite(blue, HIGH);
  digitalWrite(yellow, HIGH);
  //RGB LEFT 1
  ledcAttachPin(LEDR, R_channel);  
  ledcAttachPin(LEDG, G_channel);  
  ledcAttachPin(LEDB, B_channel);   
  ledcSetup(R_channel, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel, pwm_Frequency, pwm_resolution);
  //RGB LEFT 2
  ledcAttachPin(LEDR1, R_channel1);  
  ledcAttachPin(LEDG1, G_channel1);  
  ledcAttachPin(LEDB1, B_channel1);   
  ledcSetup(R_channel1, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel1, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel1, pwm_Frequency, pwm_resolution); 
  //RGB MIDDLE 1
  ledcAttachPin(LEDR2, R_channel2);  
  ledcAttachPin(LEDG2, G_channel2);  
  ledcAttachPin(LEDB2, B_channel2);   
  ledcSetup(R_channel2, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel2, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel2, pwm_Frequency, pwm_resolution);  
  //RGB MIDDLE 2
  ledcAttachPin(LEDR3, R_channel3);  
  ledcAttachPin(LEDG3, G_channel3);  
  ledcAttachPin(LEDB3, B_channel3);   
  ledcSetup(R_channel3, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel3, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel3, pwm_Frequency, pwm_resolution);  
  //RGB RIGHT 1
  ledcAttachPin(LEDR4, R_channel4);  
  ledcAttachPin(LEDG4, G_channel4);  
  ledcAttachPin(LEDB4, B_channel4);   
  ledcSetup(R_channel4, pwm_Frequency, pwm_resolution);  
  ledcSetup(G_channel4, pwm_Frequency, pwm_resolution);  
  ledcSetup(B_channel4, pwm_Frequency, pwm_resolution);      
 }  
 void loop() {  
  

  buttonPress();

  printCharge();
  
 }

  void buttonPress(){
    int switcher = 1;
    
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
    int SOC = 60;

    if (SOC == 40){
      RGB_Left1(255,0,0); // RED ccolor
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Middle(0,0,0); // No ccolor
      RGB_Right1(0,0,0); // No ccolor
      RGB_Right2(0,0,0); // No ccolor
    }
    else if (SOC == 60){
      RGB_Left1(255,0,0); // RED ccolor
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Middle(255,255,0); // yellow ccolor
      RGB_Right1(0,0,0); // No ccolor
      RGB_Right2(0,0,0); // No ccolor
    }
    else if (SOC == 80){
      RGB_Left1(255,0,0); // RED ccolor
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Middle(255,255,0); // yellow ccolor
      RGB_Right1(0,255,0); // green ccolor
      RGB_Right2(0,0,0); // No ccolor
    }
    else if (SOC == 100){
      RGB_Left1(255,0,0); // RED ccolor
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Middle(255,255,0); // yellow ccolor
      RGB_Right1(0,255,0); // green ccolor
      RGB_Right2(0,255,0); // green ccolor
    }
  }

  void printSOH(){
    int SOH = 10;

    if (SOH == 3){
      RGB_Left1(255,0,0); // RED ccolor
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Middle(0,0,0); // No ccolor
      RGB_Right1(0,0,0); // No ccolor
      RGB_Right2(0,0,0); // No ccolor
    }
    else if (SOH == 5){
      RGB_Left1(255,0,0); // RED ccolor
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Middle(255,255,0); // yellow ccolor
      RGB_Right1(0,0,0); // No ccolor
      RGB_Right2(0,0,0); // No ccolor
    }
    else if (SOH == 7){
      RGB_Left1(255,0,0); // RED ccolor
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Middle(255,255,0); // yellow ccolor
      RGB_Right1(0,255,0); // green ccolor
      RGB_Right2(0,0,0); // No ccolor
    }
    else if (SOH == 10){
      RGB_Left1(255,0,0); // RED ccolor
      RGB_Left2(255,0,0); // RED ccolor
      RGB_Middle(255,255,0); // yellow ccolor
      RGB_Right1(0,255,0); // green ccolor
      RGB_Right2(0,255,0); // green ccolor
    }
  }

  void printCharge(){
  int batterylevel = 100;
    if (batterylevel = 100){
      digitalWrite(blue, HIGH); // turn the LED on by making the voltage HIGH
      digitalWrite(yellow, LOW); // turn the LED off by making the voltage LOW
    }
    else{
      digitalWrite(blue, LOW); // turn the LED on by making the voltage HIGH
      digitalWrite(yellow, HIGH); // turn the LED off by making the voltage LOW
    }
  }

  void printAlarm(){
    int alarm = 0;
    if (alarm == 0){
      //no alarm means no change in UI
      digitalWrite(red, LOW); // turn the LED on by making the voltage HIGH
    }
    else{
      digitalWrite(red, HIGH); // turn the LED on by making the voltage HIGH
      if (alarm == 1){
        RGB_Left1(255,255,0); // yellow ccolor
        RGB_Left2(255,255,0); // yellow ccolor
        RGB_Middle(255,255,0); // yellow ccolor
        RGB_Right1(255,255,0); // yellow ccolor
        RGB_Right2(255,255,0); // yellow ccolor
      }
      else if (alarm == 2){
        RGB_Left1(255,0,0); // RED ccolor
        RGB_Left2(255,0,0); // RED ccolor
        RGB_Middle(255,0,0); // RED ccolor
        RGB_Right1(255,0,0); // RED ccolor
        RGB_Right2(255,0,0); // RED ccolor
      }
      else if (alarm == 3){
        RGB_Left1(0,255,0); // green ccolor
        RGB_Left2(0,255,0); // green ccolor
        RGB_Middle(0,255,0); // green ccolor
        RGB_Right1(0,255,0); // green ccolor
        RGB_Right2(0,255,0); // green ccolor
      }
      else if (alarm == 4){
        RGB_Left1(255,192,203); // pink ccolor
        RGB_Left2(255,192,203); // pink ccolor
        RGB_Middle(255,192,203); // pink ccolor
        RGB_Right1(255,192,203); // pink ccolor
        RGB_Right2(255,192,203); // pink ccolor
      }
      else if (alarm == 5){
        RGB_Left1(255,69,0); // orange ccolor
        RGB_Left2(255,69,0); // orange ccolor
        RGB_Middle(255,69,0); // orange ccolor
        RGB_Right1(255,69,0); // orange ccolor
        RGB_Right2(255,69,0); // orange ccolor
      }
      else if (alarm == 6){
        RGB_Left1(0,0,255); // blue ccolor
        RGB_Left2(0,0,255); // blue ccolor
        RGB_Middle(0,0,255); // blue ccolor
        RGB_Right1(0,0,255); // blue ccolor
        RGB_Right2(0,0,255); // blue ccolor
      }
      else if (alarm == 7){
        RGB_Left1(255,255,255); // white ccolor
        RGB_Left2(255,255,255); // white ccolor
        RGB_Middle(255,255,255); // white ccolor
        RGB_Right1(255,255,255); // white ccolor
        RGB_Right2(255,255,255); // white ccolor
      }
      else if (alarm == 8){
        RGB_Left1(153,0,153); // purple ccolor
        RGB_Left2(153,0,153); // purple ccolor
        RGB_Middle(153,0,153); // purple ccolor
        RGB_Right1(153,0,153); // purple ccolor
        RGB_Right2(153,0,153); // purple ccolor
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
 void RGB_Middle(int i, int j, int k)  //middle 1
 {  
  ledcWrite(R_channel2, i);   
  ledcWrite(G_channel2, j);  
  ledcWrite(B_channel2, k);   
 } 
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

