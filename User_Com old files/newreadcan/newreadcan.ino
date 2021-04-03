#include <ESP32CAN.h>
#include <CAN_config.h>

CAN_device_t CAN_cfg;






int soc;
int currentLED;
int supplyLED;
int powerSupplyUnder;
int powerSupplyOver;


void setup() {
  Serial.begin(115200);
  Serial.println("iotsharing.com CAN demo");
  CAN_cfg.speed = CAN_SPEED_250KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_5;
  CAN_cfg.rx_pin_id = GPIO_NUM_4;
  CAN_cfg.rx_queue = xQueueCreate(10, sizeof(CAN_frame_t));
  //start CAN Module
  ESP32Can.CANInit();
}

void loop() {
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
            if(rx_frame.data.u8[1] == 1) {
              powerSupplyUnder = 1;
            }
            //power supply over
            else if(rx_frame.data.u8[1] == 2){
              powerSupplyOver = 1;
            }
            
            
          }


      }
      //printf(" from 0x%08x, DLC %d\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);

      // printf("\n");
    }
  }
  else
  {
  }
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
