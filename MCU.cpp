#include "mbed.h"
 
Ticker ticker;
CAN can1(p30, p29);
 
int main() {
    can1.frequency(500000);

    char counter = 0x40;
    CANMessage msg=CANMessage(1337, &counter, 1);

    while(1) {

        if(can1.write(msg)){
            printf("sent\n");
        }
        
        
    }
}
