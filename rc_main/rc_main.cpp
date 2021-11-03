#include <CAN.h>
#include <LiquidCrystal.h>
#include <LV.h>


void setup() {
  Serial.begin(9600);
  
  //Bit rate = 500Kbps
  CAN.begin(500E3);

  int startup = 0;
  int bootup = 0;

  //initialize the library with the numbers of the interface pins (CHECK TINKER CAD)
  LiquidCrystal lcd(12, 11, 9, 8, 6, 4);
  lcd.begin(16, 2);

  
  //Have user start bootup check using Serial Monitor
  while(startup == 0){
    //'TYPE Y' DEPRECATED JUST RUN AUTOMATICALLY ONCE POWERED
    Serial.println("Type 'Y' to run Bootup");
    while(Serial.available() == 0){
    }
    char reply = Serial.read();

    if(reply == 'Y'){
      lcd.clear();
      lcd.print("Running Bootup, DO NOT IGNITION");

      bootup += Bootup(MC2_ID, 500);
      if (bootup == 1){
        lcd.clear();
        lcd.print("MC2: Success");}
      else{
        lcd.clear();
        lcd.print("MC2: Failure");
        //break
      }

      bootup += Bootup(MCU_ID, 500);
      
      if (bootup == 2){
        //recieved all return packets
        lcd.clear();
        lcd.print("Bootup Complete, Turn on IGNITION");
        startup = 1;
      }
      else{
        lcd.clear();
        lcd.print("Bootup Failure. Check system and run again.");
      }
      
    }
    else{
      lcd.clear();
    }
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

//Params:
//id: 100, 200, 300 etc, represents which board we are pinging
//timeout: unspecified # attempts to ping the board.
int Bootup(int id, int timeout){
  int bootup = 0;
  int counter = 0;
  int packetSize = 0;
  CAN.beginPacket(id + 99);
  CAN.write('B'); //this could be empty, since we will have boards reading ID 
  CAN.endPacket();

  /*
  1. Initializes CAN Packet, Declares/sends mesage CAN ID 
  
  2. writes to all of can
  
  3. closes the packet
  Note: This is the standard practice for sending anything over can with this lib.
  */


  //Wait for & Read Reply from MC^2
  while(packetSize == 0){
    packetSize = CAN.parsePacket();
    //Returns an int of packet size recieved
    if(packetSize){
      //If we have recieved something from CAN
      if(CAN.packetId() == (id + 98)){

        /*ID's not set in stone, FOR NOW:
        Every 100s is the board ID:
          ex) 1XX = MC^2; 2XX = MCU; 3XX = Telemetry...etc.
        X99 = Bootup packet
        X98 = Bootup return packet
        */

        //Serial.print("MC^2 = Online");
        return 1;
      }  
    }
    else{
      counter++;
      //Attempts to send/recieve X number of times, if count > X: timeout/break
      //Could interfere with CAN error handling, TODO: Figure that out.
    }

    if(counter >= timeout){
      //Serial.print("MC^2 = Offline");
      return 0;
    }
  }
  //This should never happen
  return -1;
}