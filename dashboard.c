#include <CAN.h>
#include "LV.h"
#include <ArduinoJson.h>
//temporary globals, will be replaced by batteryTemp etc.
//globals necessary so that the json is not fully reset on new data
int brake;
int accel;
int hlight;
int leftturn;
int rightturn;
int bmsValue;
int mcValue;

void setup() {
  pinMode(LEFT_PROJECTOR_LAMP, OUTPUT);
  pinMode(RIGHT_PROJECTOR_LAMP, OUTPUT);
  pinMode(LEFT_TURN_SIGNAL_FRONT, OUTPUT);
  pinMode(RIGHT_TURN_SIGNAL_FRONT, OUTPUT);
  pinMode(LEFT_TURN_SIGNAL_BACK, OUTPUT);
  pinMode(RIGHT_TURN_SIGNAL_BACK, OUTPUT);
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
      default:
        //do nothing
        break;
    }
  }
}

void jsonWrite(){
  const size_t capacity = JSON_OBJECT_SIZE(7);
  DynamicJsonDocument doc(capacity);
  doc["Brakes"] = brake;
  doc["Accelerator"] = accel;
  doc["Head Light"] = hlight;
  doc["Left Turn Signal"] = leftturn;
  doc["Right Turn Signal"] = rightturn;
  doc["BMS"] = bmsValue;
  doc["MC"] = mcValue;
  //delay statement might be needed
  serializeJson(doc, Serial);
}

void onReceive(int packetSize) {
  long recvID = CAN.packetId();
  char data[3] = {0};
  
  while (CAN.available()) {
      data[packetSize] = CAN.read();
      packetSize += 1;
  }
  int metricID = data[1];
  if (CAN.packetId == MCU_ID){
    //Brakes
    if(metricID == 0){
      brake = data[1];
    }
    //Accelerator
    else if(metricID == 1){
      accel = data[1];
    }
  }
  else if (CAN.packetId == DASH_ID){
    if(metricID == 0){
      hlight = data[1];
    }
    else if(metricID == 1){
      leftturn = data[1];
    }
    else if(metricID == 2){
      rightturn = data[1];
    }
  }
  else if (CAN.packetId == BMS_ID){
    if(data[0] = 0){
      bmsValue = data[1]
    }
  else if (CAN.packetId == MC_ID){
    if(data[0] = 0){
      mcValue = data[1]
    }
  }
  jsonWrite();
}
  
  /*if(debug){
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
    Serial.println();
  }
}
*/