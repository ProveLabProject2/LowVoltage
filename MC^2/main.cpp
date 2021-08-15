
#include <CAN.h>
#include <LV.h>
#include <Adafruit_MCP4725.h>
#include <Wire.h>


Adafruit_MCP4725 dacBoard;

void setup() {
  pinMode(DIRECTION_IN, INPUT);
  pinMode(TEMPERATURE, INPUT)
  
  pinMode(DIRECTION_OUT, OUTPUT);
  pinMode(BRAKE, OUTPUT);
  pinMode(ACCELERATOR, OUTPUT);
  
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
  //begin I2C
  Wire.begin();
    
  //begin DAC
  dacBoard.begin();
    
  // register the receive callback
  CAN.onReceive(onReceive);
}

void loop() {
  //doesn't write just receives and translates
  }
}

void onReceive(int packetSize) {
  Serial.print("Received ");
  	if (CAN.packetID() == MC_ID){
  		unsigned char pedalType = (char)CAN.read();
      unsigned char outByte = (char)CAN.read();
  		
  		if(pedalType == 0x00){
  		  digitalWrite(BRAKE, outByte);
  		} else 
        outByte = outByte * 16;
        dacBoard.setVoltage(outByte);
  		}
  	}
}
