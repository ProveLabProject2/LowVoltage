#include <CAN.h>
#include <LV.h>

void setup() {
  if(debug){
    Serial.begin(9600);
    while (!Serial);
    
  
  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    if(debug)
      Serial.println("Starting CAN failed!");
    while (1);
  }

  // register the receive callback
  CAN.onReceive(onReceive);
}

void loop() {
  // do nothing
}

void onReceive(int packetSize) {
  long recvID = CAN.packetId();
  char data[100];
  
  int i = 0;
  
  while (CAN.available()) {
      data[i] = CAN.read();
      i += 1;
  }
  
  if(debug){
    Serial.print("Received ");
    if (CAN.packetExtended()) {
      Serial.print("extended ");
    }
    Serial.print("packet with id 0x");
    Serial.print(recvID, HEX);
    
    for(i = 0; i < packetSize; i++){
      Serial.print("%c", data[i]);
    }
    
    Serial.println('\n');
  }else{
    System.print("%c", recvID);
    for(i = 0; i < packetSize; i++){
      Serial.print("%c", data[i]);
    }
    System.println();
  }
}