#include "mbed.h"


SDFileSystem sd(PB_15, PB_14, PB_13, PB_12, "sd"); // MOSI, MISO, SCK, CS



void setupInterrupts(){
  can.attach(&canISR, CAN::RxIrq);
}



void canISR(){
  CAN_IER &= ~CAN_IER_FMPIE0;                 // disable RX interrupt
  queue.call(&canHandler);                    // add canHandler() to events queue
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
      msg >> rpm_hz;
  }
  
  else if(msg.id == TEMPERATURE_ID)
  {
      msg >> temp_motor;
  }
  
  else if (msg.id == FLAGS_ID)
  {
      msg >> flags;
  }
  else if (msg.id == THROTTLE_ID)
  {
      switch_clicked = true;
      state_buffer.push( );
      msg >> switch_state;
  }
  else if (msg.id == IMU_ACC_ID)
  {
      imu_last_acq = t.read_ms();
      msg >> data.imu[imu_counter].acc_x >> data.imu[imu_counter].acc_y >> data.imu[imu_counter].acc_z;
  }

  else if (msg.id == IMU_DPS_ID)
  {
      msg >> data.imu[imu_counter].dps_x >> data.imu[imu_counter].dps_y >> data.imu[imu_counter].dps_z;
      if (imu_counter < 3)
      {
          imu_counter++;
      }
      else if (imu_counter == 3)
      {
          imu_buffer.push(data.imu);
          imu_counter = 0;
      }
  }
  else if (msg.id == SPEED_ID)
  {
      msg >> data.speed;
  }
}



int main() {


    CANMsg txMsg;
    eventThread.start(callback(&queue, &EventQueue::dispatch_forever));
    setupInterrupts();


    //Initialize SD
    while(sd.disk_status()){
        lcd.printf("sd.disk_status");
        sd.disk_initialize();
        myled = 0;
        wait(0.5);
        myled = 1;
        wait(0.2);
    }          


  while(1) {
  switch (current_state)
        {
            case IDLE_ST:
                break;
            default:
                break;
        }
    }
  }
}