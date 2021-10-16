This repo contains the code that runs on the Arduinos.

Listed below is a description of the functionality of each file.

*This is a living document and new files should be added to this README with its description*


## rc_main.c
This file is the main C code that will run on the primary arduino board on the rolling chassis. All output will be displayed to an LCD display. As of 9/29/2021 the primary function of this board is to create a heartbeat signal to ensure the functionality of all other boards before MILA is permitted to power on.

### Setup()
The setup function works as follows:
- Initialize both a serial output and CAN at 500 kbps
- Wait for the user to confirm they are ready to start
- Check the status of the MC^2 and the MCU using Bootup()
- If all boards are ok, end the setup loop: START THE CAR
- Else: tells the user to check the systems and requires them to hit start again. 
- TODO: Add a timeout so the failure warning prints for longer instead of being immediatly overwritten. 
### Bootup()
- Params:
  - id: int in increments of 100 representing the different external boards
  - timeout: int representing # of times to attempt to connect to the external board.
- Initializes CAN Packet, Declares/sends mesage CAN ID 
- Writes any message to all of CAN
- Closes the packet
  - Note: The above three steps are the standard practice for sending anything over CAN.
- In a loop awaiting reply from the target board:
- Checks if we have recieved a packet, and if it is from the target board
  - If the reply packet is from the right board we return 1
- otherwise we retry until hitting the timeout counter, and return 0

## LV.h
LV.h contains all our globally defined variables such as identifiers for boards and lights.
## LV.c
LV.c contains methods used to format CAN messages to CANOpen protocol.
## MC.c
MC.c contains all code for the MC^2 board. The MC^2 board will take in 2 switches from the Dash interaction(ignition, forward/neutral/reverse) as well as CAN data from the MCU regarding pedal and brake sensors. It outputs directly to the motor controller after translating the inputs into readable packets. Right now it directly outputs via a DAC board, but will eventually transition to DS402 protocol.
## dashboard.c
Dashboard.c will not be used on the rolling chassis, but translates CAN data into parsable data for the tablet. Will be adjusted based on testing at the RC stage.
## mcu.c
MCU.c takes in the pedal positions and sends them over CAN, as well as turning on headlights and taillights eventually. It is constantly reading the acceleration until interrupted by the brake. When cruise control is added, cruise control will take priority over acceleration, but the brake will maintain interrupt status. 
## rc_main.c
rc_main.c is the main interaction point of the Rolling Chassis. It runs heartbeat system and is contantly printing CAN data to the LCD display.
## peripheral.c
Peripheral acts as a dummy template for any additionally added sensors. The BMS, for example will use a similar set up with the necessary added methods to communicate over CAN with the rest of the system.
## node_generic.c
node_generic.c is a genric node for the heartbeat testing, if a CAN node is added that needs to be checked in the heartbeat, for example, the BMS again, it will use this code.

