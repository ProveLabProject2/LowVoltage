#include <CAN.h>
#include <LV.h>

unsigned char sensor_1_val = 0;

void setup() {
  pinMode(SENSOR_1, INPUT);

  Serial.begin(9600);
  while (!Serial);

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
  unsigned char current_sensor_1_val = analogRead(SENSOR_1) >> 2;
  
  if(current_sensor_1_val - sensor_1_val > SENSOR_1_THRESH){
    CAN.beginPacket(PERIPH_ID);
    CAN.write(current_sensor_1_val, 1);
    CAN.endPacket();
    sensor_1_val = current_sensor_1_val
  }

  delay(100);
}

void onReceive(int packetSize) {
  // Arbitrary, doesnt do anything for now.
}