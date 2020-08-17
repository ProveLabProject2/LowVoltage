#include <CAN.h>
#include <LV.h>

define LEFT_PROJECTOR_LAMP 3;
define RIGHT_PROJECTOR_LAMP 5;
define LEFT_TURN_SIGNAL_FRONT 6;
define RIGHT_TURN_SIGNAL_FRONT 9;
define LEFT_TURN_SIGNAL_BACK 10;
define RIGHT_TURN_SIGNAL_BACK 11;

void setup() {
  pinMode(LEFT_PROJECTOR_LAMP, OUTPUT);
  pinMode(RIGHT_PROJECTOR_LAMP, OUTPUT);
  pinMode(LEFT_TURN_SIGNAL_FRONT, OUTPUT);
  pinMode(RIGHT_TURN_SIGNAL_FRONT, OUTPUT);
  pinMode(LEFT_TURN_SIGNAL_BACK, OUTPUT);
  pinMode(RIGHT_TURN_SIGNAL_BACK, OUTPUT);

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
  unsigned char incomingByte = 0;
  unsigned char light = 0;
  unsigned char value = 0;

  // Receive light control byte. Value has 5 bit resolution
  if (Serial.available()){
    incomingByte = Serial.read();
    light = incomingByte >> 5;
    value = (incomingByte << 3) >> 3;
    switch(light){
      case 0:
        pinMode(LEFT_PROJECTOR_LAMP, value);
      case 1:
        pinMode(RIGHT_PROJECTOR_LAMP, value);
      case 2:
        pinMode(LEFT_TURN_SIGNAL_FRONT, value);
      case 3:
        pinMode(RIGHT_TURN_SIGNAL_FRONT, value);
      case 4:
        pinMode(LEFT_TURN_SIGNAL_BACK, value);
      case 5:
        pinMode(RIGHT_TURN_SIGNAL_BACK, value);
      case default:
        continue;
    }
  }
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
    Serial.print("%c", recvID);
    for(i = 0; i < packetSize; i++){
      Serial.print("%c", data[i]);
    }
    System.println();
  }
}