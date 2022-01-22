#include "LV.h"
#include<stdint.h>
#include <CAN.h>


// Will be used for sending commands or requests to motor controller / BMS
// Another packet format will be used to send requests between other nodes
void sendCANOpenSDO(uint8_t nodeID, bool write, uint16_t odIndex, uint8_t odSubIndex, int nBytes, uint8_t* bytes) {
  uint16_t cobID = nodeID | (0x600);
  uint8_t odIndexLSB = (uint8_t) odIndex;
  uint8_t odIndexMSB = (odIndex >> 8);

  // Command Byte Construction
  // CCS   Toggle   n   e   s
  // 001     0     11   1   1
  // Command client specifier (3 bits)
  // If we're reading data from the motor controller (SDO Upload), CCS = 2
  // If we're writing data to the motor controller (SDO Download), CCS = 1
  uint8_t commandByte = 2;
  if(write)
    commandByte = 1;

  int dataLength;

  // Limit length of data to 4 bytes
  if (nBytes > 4)
    dataLength = 4;

  commandByte = commandByte << 5;
  commandByte = commandByte | ((4 - dataLength) << 2);
  commandByte = commandByte | 3;


  uint8_t data[8] = { 0 };
  data[0] = commandByte;
  data[1] = odIndexLSB;
  data[2] = odIndexMSB;
  data[3] = odSubIndex;

  // Little Endian or Big Endian?
  // Currently set up little endian
  if(nBytes > 0 && bytes != NULL) {
    for(int i = 0; i < dataLength; i++) {
      data[3 + i] = bytes[i];
    }
  }

  CAN.beginPacket(cobID);
  CAN.write(data, 8);
  CAN.endPacket();
}

// Checks to see if there's a CANOpen packet for a given nodeID, will write
// the data of the packet to the provided corresponding pointers, and return length
int receiveCANOpenSDO(uint8_t nodeID, uint16_t* odIndex, uint8_t* odSubIndex, uint8_t* bytes) {
  if(( (int) packetSize = CAN.parsePacket()) != 0) {
    long id = CAN.packetId();

    // If this is a response packet from the node with nodeID
    if( (0x580 | nodeID) == id) {
      // Parse the object dictionary index sub index (that will hopefully be mapped here later)
      uint8_t commandByte = CAN.read(); // unused
      uint8_t odIndexLSB = CAN.read();
      uint8_t odIndexMSB = CAN.read();

      // Assigns values of pointers to received odIndex, and odSubIndex
      *odIndex = (odIndexMSB << 8) | odIndexLSB;
      *odSubIndex = CAN.read();

      int nBytes = 0;
      while(CAN.available() > 0) {
        bytes[nBytes] = CAN.read();
        nBytes++;

      }
      return nBytes;
    }
  }
  // If no matching packet sent, return -1
  return -1;
}
