// Code structure taken from Radio Head Library.
// Server side of telemetry, receives n-byte message and 

// Consult library Radio Head/rf95/rf95server.ino file for detailed comments. 
// rf95_server.pde

#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95; // Singleton instance of the radio driver

int led = 8;

void setup()
{
  pinMode(led, OUTPUT);    
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // You can change the modulation parameters with eg
  rf95.setModemConfig(RH_RF95::Bw125Cr45Sf2048);
  /* Available configurations.
  Bw125Cr45Sf128 = 0,    ///< Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
  Bw500Cr45Sf128,            ///< Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
  Bw31_25Cr48Sf512,    ///< Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
  Bw125Cr48Sf4096,           ///< Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, low data rate, CRC on. Slow+long range
  Bw125Cr45Sf2048,           ///< Bw = 125 kHz, Cr = 4/5, Sf = 2048chips/symbol, CRC on. Slow+long range
   */
  rf95.setFrequency(915.0);  //Setting frequency to board spec.
  rf95.setTxPower(14, false); //Setting power based on board spec. 
}

void loop()
{
  if (rf95.available()) //Looping for 
  {
    // Should be a message for us now  
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      digitalWrite(led, HIGH);
      Serial.print("got request: ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("recv failed"); //Fail if once we have receive 
    }
  }
}
