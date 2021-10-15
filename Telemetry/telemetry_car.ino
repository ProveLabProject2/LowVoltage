// Code structure taken from Radio Head Library.
// Server side of telemetry, receives n-byte message and 
// Consult library Radio Head/rf95/rf95server.ino file for detailed comments. 
// rf95_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client

#include <SPI.h>
#include <RH_RF95.h>


RH_RF95 rf95; // Singleton instance of the radio driver

void setup() 
{

  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
  Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // You can change the modulation parameters using setups below. 
  rf95.setModemConfig(RH_RF95::Bw125Cr45Sf2048);
  /*
   Bw125Cr45Sf128 = 0,    ///< Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
  Bw500Cr45Sf128,            ///< Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
  Bw31_25Cr48Sf512,    ///< Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
  Bw125Cr48Sf4096,           ///< Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, low data rate, CRC on. Slow+long range
  Bw125Cr45Sf2048,           ///< Bw = 125 kHz, Cr = 4/5, Sf = 2048chips/symbol, CRC on. Slow+long range
   */
  rf95.setFrequency(915.0);
  rf95.setTxPower(14, false); // False uses PA Boost Pins true uses PF RFO pins
}

void loop()
{
  Serial.println("Sending to Home Base");
  
  uint8_t data[] = "123456789X"; // Send message to rf95_server, Want to send 20 bytes, each char is 1 byte.
  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; 
  uint8_t len = sizeof(buf);
  //delay(25);  Apart from normal computation delay, we can adjust this delay if we need more reliability. 
}
