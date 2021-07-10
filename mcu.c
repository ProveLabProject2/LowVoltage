
#include "mbed.h"
#include "LV.h"
DigitalOut l_projector_lamp (p26);
DigitalOut r_projector_lamp (p25);
DigitalOut r_dtr (p24);
DigitalOut l_dtr (p19);
DigitalOut r_tail (p23);
DigitalOut l_tail (p20);
DigitalOut r_turnsignal (p21);
DigitalOut l_turnsignal (p22);

InteruptIn brake (p17);
AnalogIn accelerator (p18);

CAN can1 (p30, p29);

Ticker accelerator_read;

void brake_switch_handler_on(){
    // Dont know what this is, change to defines. Standardize a format for data being transmitted through CAN. Add defines instead of hardcoded values
    can1.write(CANMessage(0x02,[0x01], 1));
    r_tail = 1;
    l_tail = 1;
}

void brake_switch_handler_off(){
    // Dont know what this is, change to defines. Standardize a format for data being transmitted through CAN. Add defines instead of hardcoded values
    can1.write(CANMessage(0x02,[0x00], 1));
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
    brake.rise(&brake_switch_handler_on);
    brake.fall(&brake_switch_handler_off);

    accelerator_read.attach(&accelerator_time_handler, 0.1);
    // Probably have to add a empty while loop here
}



