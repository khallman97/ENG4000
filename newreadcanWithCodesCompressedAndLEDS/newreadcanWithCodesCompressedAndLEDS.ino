#include <ESP32CAN.h>
#include <CAN_config.h>

CAN_device_t CAN_cfg;
#define LEDR 25 // RED pin of rgb led is connected to 25 gpio pin  
#define LEDG 26 // green pin is connected to 26 gpio  
#define LEDB 27 // left first

#define LEDR1 16 // RED pin of rgb led is connected to 25 gpio pin  
#define LEDG1 17 // green pin is connected to 26 gpio  
#define LEDB1 2 // left 2 second

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





int soc;
int currentLED;
int supplyLED;
int powerSupplyUnder;
int powerSupplyOver;


void setup() {
  startCAN();
  initLEDS();

}

void loop() {
  readCAN();

  if (digitalRead(buttonPin)) {
    if (switcher == 1) {
      switcher = 0;
    } else {
      switcher = 1;
    }
  }

  buttonPress();

}

void buttonPress() {

  //
  if (switcher == 0) {
    printSOC();
  }
  else if (switcher == 1) {
    printSOH();
  }
  else {
    printAlarm();
  }
}

void startCAN() {
  Serial.begin(115200);
  Serial.println("iotsharing.com CAN demo");
  CAN_cfg.speed = CAN_SPEED_250KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_5;
  CAN_cfg.rx_pin_id = GPIO_NUM_4;
  CAN_cfg.rx_queue = xQueueCreate(10, sizeof(CAN_frame_t));
  //start CAN Module
  ESP32Can.CANInit();
}

void readCAN() {
  CAN_frame_t rx_frame;
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {
    if (rx_frame.FIR.B.RTR == CAN_RTR)
      printf(" RTR from 0x%08x, DLC %d\r\n", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
    else {
      //might need to change byte orders around
      if (rx_frame.MsgID == 0x1835F4E8) {
        printf("SOC\n");
        if (rx_frame.data.u8[0] == 2) {
          printf("%d \n", rx_frame.data.u8[2]);
          //Assign SOC value
          soc = rx_frame.data.u8[2];

          //Assign current Value
          unsigned int precur = (rx_frame.data.u8[3] << 8) | rx_frame.data.u8[4];
          int current = (precur / 10) - 1600;
          if (current > 2) {
            currentLED = 1;
          }

          //Assign supply voltage
          unsigned int prevol = (rx_frame.data.u8[5] << 8) | rx_frame.data.u8[6];
          if (prevol > 2) {
            supplyLED = 1;
          }
          //Add  di2
        }
        size_t n = sizeof(rx_frame.data.u8) / sizeof(rx_frame.data.u8[0]);
        for (int i = 0; i < n; i++) {

          printf("%d\t", (int) rx_frame.data.u8[i]);
        }
        printf("\n");
      }
      //Alarms
      else if (rx_frame.MsgID == 0x1838F4E8) {
        //byte 0 = 3 general alarms
        if (rx_frame.data.u8[0] == 2) {
          //power supply under
          if (rx_frame.data.u8[1] == 1) {
            powerSupplyUnder = 1;
          }
          //power supply over
          else if (rx_frame.data.u8[1] == 2) {
            powerSupplyOver = 1;
          }
          //Add other alarms

        }
      }
    }
  }
}


void printSOC() {
  //int SOC = 100;
  if (SOC <= 10 ) {
    RGB_Right2(255, 255, 0); // Yellow
    RGB_Right1(255, 0, 0); // Red
    RGB_Left2(255, 0, 0); // RED ccolor
    RGB_Left1(255, 0, 0); // RED ccolor
  }
  else if (SOC > 10 && SOC <= 20) {
    RGB_Right2(0, 255, 0); // green
    RGB_Right1(255, 0, 0); // Red
    RGB_Left2(255, 0, 0); // RED ccolor
    RGB_Left1(255, 0, 0); // RED ccolor
  }
  else if (SOC > 20 && SOC <= 50) {
    RGB_Right2(0, 255, 0); // green
    RGB_Right1(0, 255, 0); // green
    RGB_Left2(255, 0, 0); // RED ccolor
    RGB_Left1(255, 0, 0); // RED ccolor
  }
  else if (SOC > 50 && SOC <= 80) {
    RGB_Right2(0, 255, 0); // green
    RGB_Right1(0, 255, 0); // green
    RGB_Left2(0, 255, 0); // green
    RGB_Left1(255, 0, 0); // RED ccolor
  }
  else if (SOC > 80 && SOC <= 90) {
    
    RGB_Right2(0, 255, 0); // green
    RGB_Right1(0, 255, 0); // green
    RGB_Left2(0, 255, 0); // green
    RGB_Left1(255, 255, 0); // yellow
  }
  else if (SOC > 90) {
    RGB_Right3(0, 255, 0); // green
    RGB_Right2(0, 255, 0); // green
    RGB_Right1(0, 255, 0); // green
    RGB_Left2(0, 255, 0); // green
    RGB_Left1(0, 255, 0); // green
  }
}


void printSOH() {

  if(SOH < 3){
    RGB_Right3(255, 255, 0); // Yellow
    RGB_Right2(255, 0, 0); // Red
    RGB_Right1(255, 0, 0); // Red
    RGB_Left2(255, 0, 0); // RED ccolor
    RGB_Left1(255, 0, 0); // RED ccolor
  }
  else if (SOH <= 3) {
    RGB_Right3(255, 255, 0);// Yellow
    RGB_Right2(255, 255, 0); // Yellow
    RGB_Right1(255, 0, 0); // Red
    RGB_Left2(255, 0, 0); // RED ccolor
    RGB_Left1(255, 0, 0); // RED ccolor
  }
  else if (SOH > 3 && SOH <= 5) {
    RGB_Right2(0, 255, 0); // green
    RGB_Right1(0, 255, 0); // green
    RGB_Left2(255, 0, 0); // RED ccolor
    RGB_Left1(255, 0, 0); // RED ccolor
    RGB_Right3(255, 0, 0);
  }
  else if (SOH > 5 && SOH <= 7) {
    RGB_Right2(0, 255, 0); // green
    RGB_Right1(0, 255, 0); // green
    RGB_Left2(0, 255, 0); // green
    RGB_Left1(255, 0, 0); // RED ccolor
    RGB_Right3(255, 0, 0);
  }
  else if (SOH > 7 && SOH <= 9) {
    RGB_Right2(0, 255, 0); // green
    RGB_Right1(0, 255, 0); // green
    RGB_Left2(0, 255, 0); // green
    RGB_Left1(0, 255, 0); // green
    RGB_Right3(255, 0, 0);
  }
  else if (SOH > 9) {
    RGB_Right2(0, 255, 0); // green
    RGB_Right1(0, 255, 0); // green
    RGB_Left2(0, 255, 0); // green
    RGB_Left1(0, 255, 0); // green
    RGB_Right3(0, 255, 0);
  }
}

void printCharge() {
  int batterylevel = 100;
  if (supplyVoltage == 1) {
    digitalWrite(blue, HIGH); // turn the LED on by making the voltage HIGH
    if (charging == 1) {
      digitalWrite(yellow, HIGH); // turn the LED off by making the voltage LOW
    }
  }
}

void printAlarm() {
  int alarm = 0;
  if (alarm == 0) {
    //no alarm means no change in UI
    digitalWrite(red, LOW);
  }
  else {
    digitalWrite(red, HIGH); // turn the LED on by making the voltage HIGH
    if (alarm == 1) {
      RGB_Left1(255, 255, 0); // yellow ccolor
      RGB_Left2(255, 255, 0); // yellow ccolor
      RGB_Right3(255, 255, 0); // yellow ccolor
      RGB_Right1(255, 255, 0); // yellow ccolor
      RGB_Right2(255, 255, 0); // yellow ccolor
    }
    else if (alarm == 2) {
      RGB_Left1(255, 0, 0); // RED ccolor
      RGB_Left2(255, 0, 0); // RED ccolor
      RGB_Right3(255, 0, 0); // RED ccolor
      RGB_Right1(255, 0, 0); // RED ccolor
      RGB_Right2(255, 0, 0); // RED ccolor
    }
    else if (alarm == 3) {
      RGB_Left1(0, 255, 0); // green ccolor
      RGB_Left2(0, 255, 0); // green ccolor
      RGB_Right3(0, 255, 0); // green ccolor
      RGB_Right1(0, 255, 0); // green ccolor
      RGB_Right2(0, 255, 0); // green ccolor
    }
    else if (alarm == 4) {
      RGB_Left1(255, 192, 203); // pink ccolor
      RGB_Left2(255, 192, 203); // pink ccolor
      RGB_Right3(255, 192, 203); // pink ccolor
      RGB_Right1(255, 192, 203); // pink ccolor
      RGB_Right2(255, 192, 203); // pink ccolor
    }
    else if (alarm == 5) {
      RGB_Left1(255, 69, 0); // orange ccolor
      RGB_Left2(255, 69, 0); // orange ccolor
      RGB_Right3(255, 69, 0); // orange ccolor
      RGB_Right1(255, 69, 0); // orange ccolor
      RGB_Right2(255, 69, 0); // orange ccolor
    }
    else if (alarm == 6) {
      RGB_Left1(0, 0, 255); // blue ccolor
      RGB_Left2(0, 0, 255); // blue ccolor
      RGB_Right3(0, 0, 255); // blue ccolor
      RGB_Right1(0, 0, 255); // blue ccolor
      RGB_Right2(0, 0, 255); // blue ccolor
    }
    else if (alarm == 7) {
      RGB_Left1(255, 255, 255); // white ccolor
      RGB_Left2(255, 255, 255); // white ccolor
      RGB_Right3(255, 255, 255); // white ccolor
      RGB_Right1(255, 255, 255); // white ccolor
      RGB_Right2(255, 255, 255); // white ccolor
    }
    else if (alarm == 8) {
      RGB_Left1(153, 0, 153); // purple ccolor
      RGB_Left2(153, 0, 153); // purple ccolor
      RGB_Right3(153, 0, 153); // purple ccolor
      RGB_Right1(153, 0, 153); // purple ccolor
      RGB_Right2(153, 0, 153); // purple ccolor
    }
  }
}

void initLEDS() {
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

void RGB_Right3(int i, int j, int k)  //right 1
{
  ledcWrite(R_channel5, i);
  ledcWrite(G_channel5, j);
  ledcWrite(B_channel5, k);
}

//value = can msg, offset = start point, n = how many
unsigned getbits(unsigned value, unsigned offset, unsigned n)
{
  const unsigned max_n = CHAR_BIT * sizeof(unsigned);
  if (offset >= max_n)
    return 0; /* value is padded with infinite zeros on the left */
  value >>= offset; /* drop offset bits */
  if (n >= max_n)
    return value; /* all  bits requested */
  const unsigned mask = (1u << n) - 1; /* n '1's */
  return value & mask;
}
