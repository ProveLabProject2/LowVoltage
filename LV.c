#include "LV.h"

void sendCANOpenSDORequest(int metricCode, int canOpenID){
  uint8_t mainIndex1;
  uint8_t mainIndex2;
  uint8_t subIndex;
  
  switch (metricCode)
  {
    case '0':
      mainIndex1 = 0x0b;
      mainIndex2 = 0x10;
      subIndex = 0;
    case '1':
      mainIndex1 = 0x04;
      mainIndex2 = 0x20;
      subIndex = 0;
    case '2':
      mainIndex1 = 0x23;
      mainIndex2 = 0x20;
      subIndex = 0;
    case '3':
      mainIndex1 = 0x24;
      mainIndex2 = 0x20;
      subIndex = 0;
    case '4':
      mainIndex1 = 0x25;
      mainIndex2 = 0x20;
      subIndex = 0;
    case '5':
      mainIndex1 = 0x29;
      mainIndex2 = 0x20;
      subIndex = 0;
    case '6':
      mainIndex1 = 0xff;
      mainIndex2 = 0x60;
      subIndex = 0;
  }
  uint8_t canOpenRequest[8] = {0, 0, 0, 0, subIndex, mainIndex1, mainIndex2, 64};
  uint16_t id = canOpenID | 12<<7;
  CAN.beginPacket(id, 8);
  CAN.write(canOpenRequest, 8);
  CAN.endPacket();
}

