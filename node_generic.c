#include <CAN.h>

void setup() {
  Serial.begin(9600);
  
  //Bit rate = 500Kbps
  CAN.begin(500E3);

  int startup = 0;
  if (startup == 0){
    Bootup();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

//Generic Bootup
void Bootup(){
  int packetSize = 0;

  while(packetSize == 0){
    packetSize = CAN.parsePacket();

    if(packetSize){
      if(CAN.packetId() == 199){
        CAN.beginPacket(198);
        CAN.endPacket();

        packetSize = 0;
        break;
      }
    }
  }
}
