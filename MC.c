
#include <CAN.h>
#include <LV.h>


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
  		unsigned char out = (char)CAN.read();
  		
  		if(out == 0x00){
  		  digitalWrite(BRAKE, 0);
  		} else if(out == 0x01){
  		  digitalWrite(BRAKE, 0);
  		} else{
  		  analogWrite(ACCELERATOR, out);
  		}
  	}
}
