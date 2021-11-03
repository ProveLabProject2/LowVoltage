
#include <CAN.h>
#include <LV.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>


Adafruit_MCP4725 dacBoard;

void setup() {
  pinMode(DIRECTION_IN, INPUT);
  pinMode(TEMPERATURE, INPUT);
  
  pinMode(DIRECTION_OUT, OUTPUT);
  pinMode(BRAKE, OUTPUT);
  pinMode(ACCELERATOR, OUTPUT);
  bool startUp = false;
  //add heartbeat.  

  //begin I2C
  Wire.begin();
    
  //begin DAC
  dacBoard.begin();
    
  heartbeat(MC2_ID);

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
  //IF Ignition pin HIGH:
    //startUP == true;
  // register the receive callback
  CAN.onReceive(onReceive);

}

void onReceive(int packetSize) {
  Serial.print("Received ");
  	if (CAN.packetID() == MC_ID){
      if (startUp){
    		unsigned char pedalType = (char)CAN.read();
        unsigned char outByte = (char)CAN.read();
    		
    		if(pedalType == 0x00){
    		  digitalWrite(BRAKE, outByte);
    		} 
        else {
          outByte = outByte * 16;
          dacBoard.setVoltage(outByte);
    		}
      }
  	}
}
