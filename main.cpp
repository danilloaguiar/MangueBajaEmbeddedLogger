#include "mbed.h"
#include "MangueDefs.h"

SDFileSystem sd(PB_15, PB_14, PB_13, PB_12, "sd"); // MOSI, MISO, SCK, CS
DigitalOut led(PC_13);


packet_t data;
state_t state = OPEN;

int count = 0;


void setupInterrupts(){
  can.attach(&canISR, CAN::RxIrq);
}



void canISR(){
    CAN_IER &= ~CAN_IER_FMPIE0;                 // disable RX interrupt
    state = CAN;
}



void canHandler(){
  CANMsg rxMsg;
  can.read(rxMsg);
  filterMessage(rxMsg);
  CAN_IER |= CAN_IER_FMPIE0;                  // enable RX interrupt
}



void filterMessage(CANMsg msg){

  if(msg.id == RPM_ID)
  {
      msg >> data.rpm;
  }
  
  else if(msg.id == TEMPERATURE_ID)
  {
      msg >> data.temp;
  }
  
  else if (msg.id == FLAGS_ID)
  {
      msg >> data.flags;
  }
  else if (msg.id == IMU_ACC_ID)
  {
      msg >> data.imu.acc_x >> data.imu.acc_y >> data.imu.acc_z;
  }

  else if (msg.id == IMU_DPS_ID)
  {
      msg >> data.imu.dps_x >> data.imu.dps_y >> data.imu.dps_z;
  }
  else if (msg.id == SPEED_ID)
  {
      msg >> data.speed;
  }
}


int main() {

    static FILE *fp;

    //Initialize SD
    while(sd.disk_status()){
        printf("sd.disk_status");
        sd.disk_initialize();
        led = 0;
        wait(0.2);
        led = 1;
        wait(0.2);
    }          



    CANMsg txMsg;
    setupInterrupts();
    CAN_IER &= ~CAN_IER_FMPIE0;                 // disable RX interrupt
    led = 0;


  while(1) {
    switch (state)
            {
                case OPEN:
                    fp = fopen("/sd/DADOS.txt", "a");
                    if (fp == NULL) {  // if it can't open the file then print error message
                        led = 1;
                        printf("fp = null");
                    }
                    CAN_IER |= CAN_IER_FMPIE0;                  // enable RX interrupt
                    break;
                case SAVE:
                    //fprintf(fp, "%d %i \n", );
                    //fwrite((void *)&data, sizeof(packet_t), 1, fp);
                    count++;
                    printf("%" PRIu16 ", %" PRIu16 ", %" PRIu16 ", %" PRIu16 ", %" PRIu16 ", %" PRIu16 ", %" PRIu16 ", %" PRIu16 ", %" PRIu8 ", %" PRIu8 "/n", data.imu.acc_x, data.imu.acc_y, data.imu.acc_z, data.imu.dps_x, data.imu.dps_y, data.imu.dps_z, data.rpm, data.speed, data.flags);                
                    if(count>200){
                        state = CLOSE;
                    }
                    break;
                case CLOSE:
                    fclose(fp);
                    CAN_IER &= ~CAN_IER_FMPIE0;                 // disable RX interrupt
                    state = OPEN;
                    break;
                case CAN:
                    canHandler();
                    state = SAVE;
                    break;
                default:
                    break;
            }
        }
    }
}