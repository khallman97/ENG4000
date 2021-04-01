#include <ESP32CAN.h>
#include <CAN_config.h>

CAN_device_t CAN_cfg;

void setup() {
    Serial.begin(115200);
    Serial.println("iotsharing.com CAN demo");
    CAN_cfg.speed=CAN_SPEED_250KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_5;
    CAN_cfg.rx_pin_id = GPIO_NUM_4;
    CAN_cfg.rx_queue = xQueueCreate(10,sizeof(CAN_frame_t));
    //start CAN Module
    ESP32Can.CANInit();
}

void loop() {
    CAN_frame_t rx_frame;
    //CAN_write_frame(0x1830E8F4);
    //receive next CAN frame from queue
    if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){

      //do stuff!
//      if(rx_frame.FIR.B.FF==CAN_frame_std)
//        printf("New standard frame");
//      else
//        printf("New extended frame");

      if(rx_frame.FIR.B.RTR==CAN_RTR)
        printf(" RTR from 0x%08x, DLC %d\r\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
      else{
        if(rx_frame.MsgID == 0x1835F4E8){
          printf("SOC\n");
          if(rx_frame.data.u8[0] == 2){
            printf("%d \n",rx_frame.data.u8[2]);
          }
          size_t n = sizeof(rx_frame.data.u8) / sizeof(rx_frame.data.u8[0]);
          for(int i = 0; i < n; i++){
            
            printf("%d\t", (int) rx_frame.data.u8[i]);
          }
          printf("\n");
        }
        //printf(" from 0x%08x, DLC %d\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        
       // printf("\n");
      }
    }
    else
    {
//      rx_frame.FIR.B.FF = CAN_frame_std;
//      rx_frame.MsgID = 0x1830E8F4;
//      rx_frame.FIR.B.DLC = 1;
//      rx_frame.data.u8[0] = '0x0500000000000000';
//      ESP32Can.CANWriteFrame(&rx_frame);
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
