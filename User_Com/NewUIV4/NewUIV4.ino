#include <ESP32CAN.h>
#include <CAN_config.h>
#include <string.h>
#include <Arduino.h>
#include <FastLED.h>

/* Fast LED Defines */
#define LED_PIN 22 //RGB LED
#define NUM_LEDS 5 // 5 leds on strip
CRGB leds[NUM_LEDS];

/* CAN defines */
CAN_device_t CAN_cfg;
#define pwm_Frequency 5000 // pwm frequency  
#define pwm_resolution 8 // 8 bit resolution  

/*RGB LED Defines */
#define LEDR 25 // alarm led
#define LEDG 26   
#define LEDB 27  

#define LEDR1 16 // charging led  
#define LEDG1 17
#define LEDB1 2 

/* RGB Channels */
//Alarm LED
#define R_channel 0
#define G_channel 1
#define B_channel 2
//Charging
#define R_channel1 3
#define G_channel1 4
#define B_channel1 5

/* Define Button */
#define buttonPin 15

/* VAR Defines */
int SOC = 100;
int currentLED = 0;
int supplyLED = 0;
int powerSupplyUnder;
int powerSupplyOver;
int switcher = 0;
int SOH = 10;
int charging = 0;
int supplyVoltage = 0;

/* Alarm Defines */
int alarmOn = 0;
int BatteryCellOverTemperatureG = 0;
int BatteryCellUnderTemperatureG = 0;
int BatteryCellOverVoltageG = 0;
int BatteryCellUnderVoltageG = 0;
int BatteryPackUnderVoltageG = 0;
int BatteryPackOverVoltageG = 0;
int FastChargingCurrentG = 0;
int SlowChargingCurrentG = 0;
int BatteryCellUnderTemperatureS = 0;
int BatteryCellOverTemperatureS = 0;
int BatteryCellOverVoltageS = 0;
int BatteryCellUnderVoltageS = 0;
int BatteryPackUnderVoltageS = 0;
int BatteryPackOverVoltageS = 0;
int FastChargingCurrentS = 0;
int SlowChargingCurrentS = 0;
int firstCheck = 0;
int secondCheck =0;


void setup() {
  // put your setup code here, to run once:
  FastLedSetup();
  startCAN();
  initLEDS();
}

void loop() {
  // put your main code here, to run repeatedly:
    readCAN();

  if (digitalRead(buttonPin)) {
//    if (switcher == 1) {
//      switcher = 0;
//    } else {
//      switcher = 1;
//    }
    printSOH();
    //AlarmLED(255,255,255);
  } else {
     printSOC();
     //ChargingLED(255,56,56);
  }
  printAlarm();
  //buttonPress();
  printCharge();
}



void FastLedSetup(){
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5,500); 
  FastLED.clear();
  FastLED.show();
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

void buttonPress() {

  if (switcher == 0 && alarmOn == 0) {
    printSOC();
  }
  else if (switcher == 1 && alarmOn == 0) {
    printSOH();
  }
  else {
    printAlarm();
  }
}

void readCAN() {
  CAN_frame_t rx_frame;
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {
    if (rx_frame.FIR.B.RTR == CAN_RTR)
      printf(" RTR from 0x%08x, DLC %d\r\n", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
    else {
      //might need to change byte orders around
      //  printf("frmaeid %x\n",rx_frame.MsgID);
      if (rx_frame.MsgID == 0x1835F4E8) {
        //printf("SOC\n");
        if (rx_frame.data.u8[0] == 1) {
          //Assign current Value
          unsigned int precur = (rx_frame.data.u8[4] << 8) | rx_frame.data.u8[5];
          int current = (precur / 10) - 1600;
          if (current > 2) {
            currentLED = 1;
          }
          //printf("current raw hex = %x \n", precur);
          //  printf("current = %d \n", currentLED);
        }
        else if (rx_frame.data.u8[0] == 2) {
          //printf("%d \n", rx_frame.data.u8[2]);
          //Assign SOC value
          SOC = rx_frame.data.u8[3];
          //  printf("SOC = %d \n", SOC);



          //Assign supply voltage
          unsigned int prevol = (rx_frame.data.u8[6] << 8) | rx_frame.data.u8[7];
          int supplyVOL = (int)prevol / 10;
          if (supplyVOL > 2) {
            supplyLED = 1;
          }
          // printf("current = %d \n", supplyLED);
          //  printf(" prevol = %d \n", prevol);
          //Add  di2
        }
        //        size_t n = sizeof(rx_frame.data.u8) / sizeof(rx_frame.data.u8[0]);
        //        for (int i = 0; i < n; i++) {
        //
        //          printf("%x\t", (int) rx_frame.data.u8[i]);
        //        }
        //        printf("\n");
      }
      //Alarms
      else if (rx_frame.MsgID == 0x1838F4E8) {
        size_t n = sizeof(rx_frame.data.u8) / sizeof(rx_frame.data.u8[0]);
        for (int i = 0; i < n; i++) {

          printf("%x\t",  rx_frame.data.u8[i]);
        }
        printf("\n");
        //byte 0 = 3 general alarms
        if (rx_frame.data.u8[0] == 2) {
          printf("Here value for 1 is %x\n", rx_frame.data.u8[1]);
          //battery cell over temp
          if (rx_frame.data.u8[1] == 2 && rx_frame.data.u8[2] == 2 && rx_frame.data.u8[3] == 2) {
            BatteryCellOverTemperatureG = 1;
            alarmOn = 1;
          } else {
            BatteryCellOverTemperatureG = 0;
          }
          //Battery cell under temp general
          if (rx_frame.data.u8[1] == 2 && rx_frame.data.u8[2] == 4 && rx_frame.data.u8[3] == 4) {
            BatteryCellUnderTemperatureG = 1;
            alarmOn = 1;
          } else {
            BatteryCellUnderTemperatureG = 0;
          }
          //Battery Cell over voltage general
          if (rx_frame.data.u8[1] == 0x10 && rx_frame.data.u8[2] == 0x10 ) {
            printf("battery cell over\n");
            BatteryCellOverVoltageG = 1;
            alarmOn = 1;
          } else {
            BatteryCellOverVoltageG = 0;
          }
          //Battery Cell under voltage general
          if (rx_frame.data.u8[1] == 0x20 && rx_frame.data.u8[2] == 0x20 ) {
            BatteryCellUnderVoltageG = 1;
            alarmOn = 1;
          } else {
            BatteryCellUnderVoltageG = 0;
          }
          
          //Battery Pack over voltage general
          if(rx_frame.data.u8[1] == 0x10 ||rx_frame.data.u8[2] == 0 ) {
            BatteryPackOverVoltageG = 1;
            
            alarmOn = 1;
          } 
          else {
            BatteryPackOverVoltageG = 0;
          }
          //Battery Pack under voltage general
          if (rx_frame.data.u8[1] == 8  ) {
            BatteryPackUnderVoltageG = 1;
            alarmOn = 1;
          } else {
            BatteryPackUnderVoltageG = 0;
          }
          //Slow charging current
          if (rx_frame.data.u8[1] == 0x40  ) {
            SlowChargingCurrentG = 1;
            alarmOn = 1;
          } else {
            SlowChargingCurrentG = 0;
          }
          //Fast charging current
          if (rx_frame.data.u8[1] == 1 &&  rx_frame.data.u8[2] == 1) {
            FastChargingCurrentG = 1;
            alarmOn = 1;
          } else {
            FastChargingCurrentG = 0;
          }

          if (rx_frame.data.u8[1] == 0) {
            
            firstCheck = 0;
          } else {
            firstCheck = 1;
          }
        }
        //Critical Alarms
        else if (rx_frame.data.u8[0] == 1) {
          if (rx_frame.data.u8[1] == 2 && rx_frame.data.u8[2] == 2 && rx_frame.data.u8[3] == 2) {
            BatteryCellOverTemperatureS = 1;
            alarmOn = 1;
          } else {
            BatteryCellOverTemperatureS = 0;
          }
          //Battery cell under temp general
          if (rx_frame.data.u8[1] == 2 && rx_frame.data.u8[2] == 4 && rx_frame.data.u8[3] == 4) {
            BatteryCellUnderTemperatureS = 1;
            alarmOn = 1;
          } else {
            BatteryCellUnderTemperatureS = 0;
          }
          //Battery Cell over voltage general
          if (rx_frame.data.u8[1] == 0x10 && rx_frame.data.u8[2] == 0x10) {
            BatteryCellOverVoltageS = 1;
            printf("battery pack over\n");
            alarmOn = 1;
          } else {
            BatteryCellOverVoltageS = 0;
          }
          //Battery Cell under voltage general
          if (rx_frame.data.u8[1] == 0x20 && rx_frame.data.u8[2] == 0x20) {
            BatteryCellUnderVoltageS = 1;
            alarmOn = 1;
          } else {
            BatteryCellUnderVoltageS = 0;
          }
          //Battery Pack over voltage general
          if (rx_frame.data.u8[1] == 0x10 && rx_frame.data.u8[2] != 0x10 ) {
            BatteryPackOverVoltageS = 1;
            
            alarmOn = 1;
          } else {
            BatteryPackOverVoltageS = 0;
          }
          //Battery Pack under voltage general
          if (rx_frame.data.u8[1] == 8) {
            BatteryPackUnderVoltageS = 1;
            alarmOn = 1;
          } else {
            BatteryPackUnderVoltageS = 0;
          }
          //Slow charging current
          if (rx_frame.data.u8[1] == 0x40) {
            SlowChargingCurrentS = 1;
            alarmOn = 1;
          } else {
            SlowChargingCurrentS = 0;
          }
          //Fast charging current
          if (rx_frame.data.u8[1] == 1 &&  rx_frame.data.u8[2] == 1) {
            FastChargingCurrentS = 1;
            alarmOn = 1;
          } else {
            FastChargingCurrentS = 0;
          }

          if (rx_frame.data.u8[1] == 0) {
            
            secondCheck = 0;
          } else {
            secondCheck = 1;
          }
        }
        if(firstCheck == 0 && secondCheck ==0) {
          alarmOn = 0;
          //digitalWrite(red, LOW);
          AlarmLED(0,0,0);
        }

        
      }
      else if (rx_frame.MsgID == 0x1834F4E8) {
        //printf("SOH stuff\n");
        if (rx_frame.data.u8[0] == 1) {
          SOH = rx_frame.data.u8[2];
          printf("SOH = %d \n", SOH);
        }
      }
    }
  }
}


void initLEDS() {
  //Alarm LED
  ledcAttachPin(LEDR, R_channel);
  ledcAttachPin(LEDG, G_channel);
  ledcAttachPin(LEDB, B_channel);
  ledcSetup(R_channel, pwm_Frequency, pwm_resolution);
  ledcSetup(G_channel, pwm_Frequency, pwm_resolution);
  ledcSetup(B_channel, pwm_Frequency, pwm_resolution);
  
  //Charging LED
  ledcAttachPin(LEDR1, R_channel1);
  ledcAttachPin(LEDG1, G_channel1);
  ledcAttachPin(LEDB1, B_channel1);
  ledcSetup(R_channel1, pwm_Frequency, pwm_resolution);
  ledcSetup(G_channel1, pwm_Frequency, pwm_resolution);
  ledcSetup(B_channel1, pwm_Frequency, pwm_resolution);
}

void AlarmLED(int i, int j, int k) //left 1
{
  ledcWrite(R_channel, i);
  ledcWrite(G_channel, j);
  ledcWrite(B_channel, k);
}
void ChargingLED(int i, int j, int k)  //left 2
{
  ledcWrite(R_channel1, i);
  ledcWrite(G_channel1, j);
  ledcWrite(B_channel1, k);
}

void printCharge() {
  int batterylevel = 100;
  if (supplyLED == 1) {
    //digitalWrite(blue, HIGH); // turn the LED on by making the voltage HIGH
    if (currentLED == 1) {
      //digitalWrite(yellow, HIGH); // turn the LED off by making the voltage LOW
      if(SOC == 100) {
        ChargingLED(0,0,255); //make blue
      } else {
        ChargingLED(255,255,0); //make yellow
      }
      
    }
  }
}

void printSOC() {
  //int SOC = 100;
  if (SOC < 10 ) {
    leds[0] = CRGB(255,255,0);
    leds[1] = CRGB(255,0,0);
    leds[2] = CRGB(255,0,0);
    leds[3] = CRGB(255,0,0);
    leds[4] = CRGB(255,0,0);
    FastLED.show();
  }
  else if (SOC >= 10 && SOC < 20) {

    leds[0] = CRGB(0,255,0);
    leds[1] = CRGB(255,0,0);
    leds[2] = CRGB(255,0,0);
    leds[3] = CRGB(255,0,0);
    leds[4] = CRGB(255,0,0);
    FastLED.show();
  }
  else if (SOC >= 20 && SOC < 40) {
    leds[0] = CRGB(0,255,0);
    leds[1] = CRGB(0,255,0);
    leds[2] = CRGB(255,0,0);
    leds[3] = CRGB(255,0,0);
    leds[4] = CRGB(255,0,0);
    FastLED.show();

  }
  else if (SOC >= 40 && SOC < 60) {
    leds[0] = CRGB(0,255,0);
    leds[1] = CRGB(0,255,0);
    leds[2] = CRGB(0,255,0);
    leds[3] = CRGB(255,0,0);
    leds[4] = CRGB(255,0,0);
    FastLED.show();
  }
  else if (SOC >= 60 && SOC < 80) {

    leds[0] = CRGB(0,255,0);
    leds[1] = CRGB(0,255,0);
    leds[2] = CRGB(0,255,0);
    leds[3] = CRGB(0,255,0);
    leds[4] = CRGB(255,0,0);
    FastLED.show();
  }
  else if (SOC >= 80) {
    leds[0] = CRGB(0,255,0);
    leds[1] = CRGB(0,255,0);
    leds[2] = CRGB(0,255,0);
    leds[3] = CRGB(0,255,0);
    leds[4] = CRGB(0,255,0);
    FastLED.show();
  }
}

void printSOH() {

  if (SOH < 3) {
    leds[0] = CRGB(255,255,0);
    leds[1] = CRGB(255,0,0);
    leds[2] = CRGB(255,0,0);
    leds[3] = CRGB(255,0,0);
    leds[4] = CRGB(255,0,0);
    FastLED.show();
  }
  else if (SOH <= 3) {
   leds[0] = CRGB(0,255,0);
    leds[1] = CRGB(255,0,0);
    leds[2] = CRGB(255,0,0);
    leds[3] = CRGB(255,0,0);
    leds[4] = CRGB(255,0,0);
    FastLED.show();
  }
  else if (SOH > 3 && SOH <= 5) {
     leds[0] = CRGB(0,255,0);
    leds[1] = CRGB(0,255,0);
    leds[2] = CRGB(255,0,0);
    leds[3] = CRGB(255,0,0);
    leds[4] = CRGB(255,0,0);
    FastLED.show();

  }
  else if (SOH > 5 && SOH <= 7) {
     leds[0] = CRGB(0,255,0);
    leds[1] = CRGB(0,255,0);
    leds[2] = CRGB(0,255,0);
    leds[3] = CRGB(255,0,0);
    leds[4] = CRGB(255,0,0);
    FastLED.show();
  }
  else if (SOH > 7 && SOH <= 9) {
     leds[0] = CRGB(0,255,0);
    leds[1] = CRGB(0,255,0);
    leds[2] = CRGB(0,255,0);
    leds[3] = CRGB(0,255,0);
    leds[4] = CRGB(255,0,0);
    FastLED.show();
  }
  else if (SOH > 9) {
   leds[0] = CRGB(0,255,0);
    leds[1] = CRGB(0,255,0);
    leds[2] = CRGB(0,255,0);
    leds[3] = CRGB(0,255,0);
    leds[4] = CRGB(0,255,0);
    FastLED.show();
  }
}

void printAlarm() {
  if (alarmOn == 0) {
    //no alarm means no change in UI
    //digitalWrite(red, LOW);
  }
  else {
     // turn the LED on by making the voltage HIGH
    if (BatteryCellOverTemperatureG  == 1) {
       AlarmLED(255,255,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(0,0,255);
       leds[2] = CRGB(0,0,255);
       leds[3] = CRGB(0,0,0);
       leds[4] = CRGB(0,0,0);
       FastLED.show();
    }
    if (BatteryCellUnderTemperatureG  == 1) {
       AlarmLED(255,255,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(0,0,255);
       leds[2] = CRGB(0,0,255);
       leds[3] = CRGB(128,0,128);
       leds[4] = CRGB(128,0,128);
       FastLED.show();
    }
    if (BatteryCellOverVoltageG  == 1) {
       AlarmLED(255,255,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(0,0,0);
       leds[2] = CRGB(0,0,0);
       leds[3] = CRGB(0,0,0);
       leds[4] = CRGB(0,0,0);
       FastLED.show();
    }
    if (BatteryCellUnderVoltageG  == 1) {
       AlarmLED(255,255,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(0,0,255);
       leds[2] = CRGB(0,0,255);
       leds[3] = CRGB(0,0,255);
       leds[4] = CRGB(0,0,0);
       FastLED.show();
    }
    if (BatteryPackUnderVoltageG  == 1) {
        AlarmLED(255,255,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(0,0,255);
       leds[2] = CRGB(0,0,255);
       leds[3] = CRGB(0,0,255);
       leds[4] = CRGB(128,0,128);
       FastLED.show();
    }
    if (FastChargingCurrentG  == 1) {
      AlarmLED(255,255,0); //yellow
      leds[0] = CRGB(0,0,255);
      leds[1] = CRGB(255,165,0);
       leds[2] = CRGB(255,165,0);
       leds[3] = CRGB(255,165,0);
       leds[4] = CRGB(255,165,0);
       FastLED.show();
    }
    if (SlowChargingCurrentG  == 1) {
     AlarmLED(255,255,0); //yellow
      leds[0] = CRGB(0,0,255);
      leds[1] = CRGB(0,0,255);
       leds[2] = CRGB(0,0,255);
       leds[3] = CRGB(0,0,255);
       leds[4] = CRGB(255,165,0);
       FastLED.show();
    }
    if (BatteryPackOverVoltageG  == 1) {
       AlarmLED(255,255,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(128,0,128);
       leds[2] = CRGB(128,0,128);
       leds[3] = CRGB(128,0,128);
       leds[4] = CRGB(128,0,128);
       FastLED.show();
    }
    if (BatteryCellUnderTemperatureS  == 1) {
       AlarmLED(255,0,0); //red
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(0,0,255);
       leds[2] = CRGB(0,0,255);
       leds[3] = CRGB(128,0,128);
       leds[4] = CRGB(128,0,128);
       FastLED.show();
    }
    if (BatteryCellOverVoltageS    == 1) {
      AlarmLED(255,0,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(0,0,0);
       leds[2] = CRGB(0,0,0);
       leds[3] = CRGB(0,0,0);
       leds[4] = CRGB(0,0,0);
       FastLED.show();
    }
    if (BatteryCellUnderVoltageS   == 1) {
      AlarmLED(255,0,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(0,0,255);
       leds[2] = CRGB(0,0,255);
       leds[3] = CRGB(128,0,128);
       leds[4] = CRGB(128,0,128);
       FastLED.show();
    }
    if (BatteryPackUnderVoltageS   == 1) {
       AlarmLED(255,0,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(0,0,255);
       leds[2] = CRGB(0,0,255);
       leds[3] = CRGB(0,0,255);
       leds[4] = CRGB(128,0,128);
       FastLED.show();
    }
    if (BatteryPackOverVoltageS   == 1) {
      AlarmLED(255,0,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(0,0,0);
       leds[2] = CRGB(0,0,0);
       leds[3] = CRGB(0,0,0);
       leds[4] = CRGB(0,0,0);
       FastLED.show();
    }
    if (FastChargingCurrentS   == 1) {
       AlarmLED(255,0,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(255,165,0);
       leds[2] = CRGB(255,165,0);
       leds[3] = CRGB(255,165,0);
       leds[4] = CRGB(255,165,0);
       FastLED.show();
    }
    if (SlowChargingCurrentS   == 1) {
       AlarmLED(255,0,0); //yellow
      leds[0] = CRGB(0,0,255);
      leds[1] = CRGB(0,0,255);
       leds[2] = CRGB(0,0,255);
       leds[3] = CRGB(0,0,255);
       leds[4] = CRGB(255,165,0);
       FastLED.show();
    }
    if (BatteryCellOverTemperatureS   == 1) {
      AlarmLED(255,0,0); //yellow
       leds[0] = CRGB(0,0,255);
       leds[1] = CRGB(0,0,255);
       leds[2] = CRGB(0,0,255);
       leds[3] = CRGB(0,0,0);
       leds[4] = CRGB(0,0,0);
       FastLED.show();
    }
  }
}



