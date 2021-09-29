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
Description Placeholder
## LV.c
Placeholder Description
## MC.c
Placeholder Description
## dashboard.c
Placeholder Description
## mcu.c
Placeholder Description
## peripheral.c
Placeholder Description

