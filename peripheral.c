#include <CAN.h>
#include <LV.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Sender");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    if(debug)
      Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  static unsigned char sensorValues[PERIPH_SENSOR_COUNT];
  static sensor sensors[PERIPH_SENSOR_COUNT] = {0};

  if(debug)
    Serial.print("Sending packet ... ");
  
  readSensor(sensorValues, PERIPH_SENSOR_COUNT, sensors)
  
  if(sendData){
    CAN.beginPacket(PERIPH_ID);
    CAN.write(sensorValues, PERIPH_SENSOR_COUNT);
    CAN.endPacket();
  }

  delay(PERIPH_SENSOR_DELAY);
}

void readSensor(unsigned char *sensorValues, unsigned char sensorCount, sensor *sensors){
  
  for(int i = 0; i < sensorCount, i++){
    if((((float)sensorValues[i]) - ((float)sensors[i].read()))/((float)sensorValues[i]) > PERIPH_SENSOR_THRESHOLD){
      sendData = true;
      sensorValues[i] = sensors[i].read();
    }
  }
}