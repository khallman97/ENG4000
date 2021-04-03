int red = 16;
int green = 17;
int blue = 5;
int yellow = 4;
int lime = 3

int buttonState = digitalRead(BUTTON_PIN);

int charged = 14;
int charging = 15;
int alarmtrigger = 18;

int switch = 1;

int batterylevel = 100 

boolean alarmON = false;
int [] alarmType = [true,true,true,true];
 
// the setup routine runs once when you press reset:
void setup()
{
// initialize the digital pin as an output.
pinMode(red, OUTPUT);
pinMode(green, OUTPUT);
pinMode(blue, OUTPUT);
pinMode(yellow, OUTPUT);
pinMode(lime, OUTPUT);
pinMode(alarmtrigger, OUTPUT);
pinMode(charged, OUTPUT);
pinMode(charging, OUTPUT);

digitalWrite(red, HIGH);
digitalWrite(green, HIGH);
digitalWrite(blue, HIGH);
digitalWrite(yellow, HIGH);
digitalWrite(lime, HIGH);
digitalWrite(alarmtrigger, HIGH);
digitalWrite(charged, HIGH);
digitalWrite(charging, HIGH);
}
 
// the loop routine runs over and over again forever:
void loop() {

  buttonPress();
  printCharge();
  
}

void buttonPress(){

  if ((alarmType[1] || alarmType[2] || alarmType[3] || alarmType[4]) == true){
    printAlarm();
  }
  else{
    if (buttonState == true){
      if (switch.equals(1)){
         printSOC();
         switch = 0;
      }
      else if (switch.equals(0)){
        printSOH();
        switch = 1;
      }
    }
  }
}

void printSOC(){
  int SOC = 40

  if (SOC == 40){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, HIGH); // turn the LED on 

    digitalWrite(blue, LOW); // turn the LED on

    digitalWrite(yellow, LOW); // turn the LED on

    digitalWrite(lime, LOW); // turn the LED on

  }
  else if (SOC == 60){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, HIGH); // turn the LED on 

    digitalWrite(blue, HIGH); // turn the LED on

    digitalWrite(yellow, LOW); // turn the LED on

    digitalWrite(lime, LOW); // turn the LED on
  }
  else if (SOC == 80){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, HIGH); // turn the LED on 

    digitalWrite(blue, HIGH); // turn the LED on

    digitalWrite(yellow, HIGH); // turn the LED on

    digitalWrite(lime, LOW); // turn the LED on
  }
  else if (SOC == 100){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, HIGH); // turn the LED on 

    digitalWrite(blue, HIGH); // turn the LED on

    digitalWrite(yellow, HIGH); // turn the LED on

    digitalWrite(lime, HIGH); // turn the LED on
  }
  
}

void printSOH(){
  int SOH = 10

  if (alarmtype == 10){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, HIGH); // turn the LED on 

    digitalWrite(blue, HIGH); // turn the LED on

    digitalWrite(yellow, HIGH); // turn the LED on

    digitalWrite(lime, HIGH); // turn the LED on
  }
  else if (SOH == 5){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, HIGH); // turn the LED on 

    digitalWrite(blue, HIGH); // turn the LED on

    digitalWrite(yellow, HIGH); // turn the LED on

    digitalWrite(lime, LOW); // turn the LED on
  }
  else if (SOH == 3){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, HIGH); // turn the LED on 

    digitalWrite(blue, HIGH); // turn the LED on

    digitalWrite(yellow, LOW); // turn the LED on

    digitalWrite(lime, LOW); // turn the LED on
  }
  else if (SOH == 1){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, LOW); // turn the LED on 

    digitalWrite(blue, LOW); // turn the LED on

    digitalWrite(yellow, LOW); // turn the LED on

    digitalWrite(lime, LOW); // turn the LED on
  }
}

void printAlarm(){
  digitalWrite(alarmtrigger, HIGH); // turn the LED on
  if (alarmType[1] == true){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, HIGH); // turn the LED on 

    digitalWrite(blue, HIGH); // turn the LED on

    digitalWrite(yellow, HIGH); // turn the LED on

    digitalWrite(lime, HIGH); // turn the LED on
  }
  else if (alarmType[2] == true){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, HIGH); // turn the LED on 

    digitalWrite(blue, HIGH); // turn the LED on

    digitalWrite(yellow, HIGH); // turn the LED on

    digitalWrite(lime, LOW); // turn the LED on
  }
  else if (alarmType[3] == true){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, HIGH); // turn the LED on 

    digitalWrite(blue, HIGH); // turn the LED on

    digitalWrite(yellow, LOW); // turn the LED on

    digitalWrite(lime, LOW); // turn the LED on
  }
  else if (alarmType[4] == true){
    digitalWrite(red, HIGH); // turn the LED on 

    digitalWrite(green, LOW); // turn the LED on 

    digitalWrite(blue, LOW); // turn the LED on

    digitalWrite(yellow, LOW); // turn the LED on

    digitalWrite(lime, LOW); // turn the LED on
  }
  
}

void printCharge(){
  if (batterylevel = 100){
    digitalWrite(charged, HIGH); // turn the LED on by making the voltage HIGH
    digitalWrite(charging, LOW); // turn the LED off by making the voltage LOW
  }
  else{
    digitalWrite(charged, LOW); // turn the LED on by making the voltage HIGH
    digitalWrite(charging, HIGH); // turn the LED off by making the voltage LOW
  }
}



