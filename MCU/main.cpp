
#include "mbed.h"
#include "LV.h"

DigitalOut l_projector_lamp (LEFT_PROJECTOR_PIN);
DigitalOut r_projector_lamp (RIGHT_PROJECTOR_PIN);
DigitalOut r_dtr (RIGHT_DTR_PIN);
DigitalOut l_dtr (LEFT_DTR_PIN);
DigitalOut r_tail (RIGHT_TAIL_PIN);
DigitalOut l_tail (LEFT_TAIL_PIN);
DigitalOut r_turnsignal (RIGHT_TURN_PIN);
DigitalOut l_turnsignal (LEFT_TURN_PIN);

InteruptIn brake (BRAKE_PIN);
AnalogIn accelerator (ACCELERATOR_PIN);

CAN.begin(500E3);

Ticker accelerator_read;
bool setup = true;

bool heartbeat(int id){
  int packetSize = 0;
  while(packetSize == 0){
    packetSize = CAN.parsePacket();

    if(packetSize){
      if(CAN.packetId() == id + 99){
        CAN.beginPacket(id + 98);
        CAN.endPacket();

        packetSize = 0;
        return false;
      }
    }
  }
}


void brake_switch_handler_on(){
    // Dont know what this is, change to defines. Standardize a format for data being transmitted through CAN. Add defines instead of hardcoded values
    can1.write(CANMessage(BRAKE_ID,[BRAKE_ON], 1));
    r_tail = 1;
    l_tail = 1;
}

void brake_switch_handler_off(){
    // Dont know what this is, change to defines. Standardize a format for data being transmitted through CAN. Add defines instead of hardcoded values
    can1.write(CANMessage(BRAKE_ID,[BRAKE_OFF], 1));
    r_tail = 0;
    l_tail = 0;
}

void accelerator_time_handler(){
    // Might have to typecase from float to char
    char accelerator_value = accelerator.read()*255;
    // Add defines and change to standard can msg format
    can1.write(CANMessage(0x02, &accelerator_value, 1));
}
 
int main() {
    while(setup){
        setup = heartbeat(MCU_ID);
    }
    brake.rise(&brake_switch_handler_on);
    brake.fall(&brake_switch_handler_off);

    accelerator_read.attach(&accelerator_time_handler, 0.1);

    while();
}



