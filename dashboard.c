#include <CAN.h>
#include <LV.h>
#include <ArduinoJson.h>
//temporary globals, will be replaced by batteryTemp etc.
//globals necessary so that the json is not fully reset on new data
int xValue;
int yValue;
int zValue;

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
      case default:
        continue;
    }
  }
}

void jsonWrite(){
  const size_t capacity = JSON_OBJECT_SIZE(3);
  DynamicJsonDocument doc(capacity);
  doc['X'] = xValue;
  doc['Y'] = yValue;
  doc['Z'] = zValue;
  //delay statement might be needed
  serializeJson(doc, Serial);
}

void onReceive(int packetSize) {
  long recvID = CAN.packetId();
  char data[3] = {0};
  int packetSize = 0;
  
  while (CAN.available()) {
      data[packetSize] = CAN.read();
      packetSize += 1;
  }
  //initializes vairables that become JSON values
  int id = data[0];
  int metricID = data[1]
  if (id = MCU_ID){
    //data comes from MCU
    if(metricID = 1){
      //metric is X metric
      xValue = data[2]
      //sets the JSON value for X metric to the new data
    }
    //Continue adding if statements here until all metrics completed
  }
  else if (id = PERIPH_ID){
    if(metricID = 1){
      yValue = data[2]
    }
  }
  else if (data[0] = BMS_ID){
    if(data[1] = 1){
      zValue = data[2]
    }
  }
  jsonWrite()
  //continue adding until all boards accounted for
  
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