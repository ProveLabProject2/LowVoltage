#include <CAN.h>


void setup() {
  Serial.begin(9600);
  
  //Bit rate = 500Kbps
  CAN.begin(500E3);

  int startup = 0;
  
  //Have user start bootup check using Serial Monitor
  while(startup == 0){
    Serial.println("Type 'Y' to run Bootup");
    while(Serial.available() == 0){
    }
    char reply = Serial.read();

    if(reply == 'Y'){
      Serial.println("Running Bootup");
      int bootup = Bootup();
      
      if (bootup == 3){
        Serial.println("Bootup Complete");
        startup = 1;
      }
      else{
        Serial.print("Bootup Failure. Check system and run again.");
      }
      
    }
    else{
      Serial.println("Please type 'Y' to run Bootup");
    }
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

int Bootup(){ 

  int bootup = 0;
  int counter = 0;
  int packetSize = 0;
  //Test MC^2
  CAN.beginPacket(199);
  CAN.write("bootup");
  CAN.endPacket();

  //Wait for & Read Reply from MC^2
  while(packetSize == 0){
    packetSize = CAN.parsePacket();
    
    if(packetSize){
      
      if(CAN.packetId() == 198){
        Serial.print("MC^2 = Online");
        bootup++;
      }  
    }
    else{
      counter++;
    }

    if(counter > 500){
      Serial.print("MC^2 = Offline");
      counter = 0;
      break;
    }
  }
  counter = 0;
  packetSize = 0;
  
  //Test MCU
  CAN.beginPacket(399);
  CAN.write("bootup");
  CAN.endPacket();

  //Wait for & Read Reply from MCU
  while(packetSize == 0){
    packetSize = CAN.parsePacket();
    
    if(packetSize){
      
      if(CAN.packetId() == 198){
        Serial.print("MCU = Online");
        bootup++;
      }  
    }
    else{
      counter++;
    }

    if(counter > 500){
      Serial.print("MCU = Offline");
      counter = 0;
      break;
    }
  }
  counter = 0;
  packetSize = 0;

  //Test Telemetry
  CAN.beginPacket(499);
  CAN.write("bootup");
  CAN.endPacket();

  //Wait for & Read Reply from Telemetry
  while(packetSize == 0){
    packetSize = CAN.parsePacket();
    
    if(packetSize){
      
      if(CAN.packetId() == 198){
        Serial.print("Telemetry = Online");
        bootup++;
      }  
    }
    else{
      counter++;
    }

    if(counter > 500){
      Serial.print("Telemetry = Offline");
      counter = 0;
      break;
    }
  }
  counter = 0;
  packetSize = 0;
  
  return bootup;
}
