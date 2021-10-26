
#include <CAN.h>
#include <LV.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>


Adafruit_MCP4725 dacBoard;

void setup() {
  pinMode(0, INPUT); //These constants need to be added to the header file
  pinMode(0, INPUT); //^
  
  pinMode(0, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(0, OUTPUT); //^
  bool startUp = false;
  //add heartbeat.  

  //begin I2C
  Wire.begin();
    
  //begin DAC
  dacBoard.begin();
    
  heartbeat(MC2_ID);

   // register the receive callback
  CAN.onReceive(onReceive);
}

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

void loop() {
  //doesn't write just receives and translates
}

void onReceive(int packetSize) {
  Serial.print("Received ");
    if (CAN.packetId() == MC_ID){ //Not sure why this is throwing an error
      unsigned char pedalType = (char)CAN.read();
      unsigned char outByte = (char)CAN.read();
      
      if(pedalType == 0x00){
        digitalWrite(BRAKE_ID, outByte);
      } 
      else {
        outByte = outByte * 16;
        dacBoard.setVoltage(outByte); //this requires more arguments
      }
    }
}
