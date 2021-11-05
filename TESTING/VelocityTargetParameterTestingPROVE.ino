  volatile byte half_revolutions;
 unsigned int rpm;
 unsigned int last_velocity;
 unsigned int acceleration_mph; 
 unsigned int velocity_mph;
 unsigned int wheel_diameter_inches;
 unsigned long timeold;
 
 void setup()
 {
   Serial.begin(115200);
   attachInterrupt(0, magnet_detect, RISING);//Initialize the intterrupt pin (Arduino digital pin 2)
   half_revolutions = 0;
   rpm = 0;
   timeold = 0;
   wheel_diameter_inches = 20; //20 inch is place holder
   acceleration_mph = 0;
   velocity_mph = 0;
   last_velocity = 0;
 }
 void loop()//Measure RPM
 {
   if (half_revolutions >= 20) { 
     rpm = 30*1000/(millis() - timeold)*half_revolutions;
     velocity_mph = rpm*wheel_diameter_inches*60*3.141592/63360; 

     acceleration_mph = 1000*(velocity_mph - last_velocity) / (millis() - timeold);
     
     timeold = millis();
     half_revolutions = 0;
     //Serial.println(rpm,DEC);
     
     last_velocity = velocity_mph; 
   }
 }
 void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   half_revolutions++;
   Serial.println("detect");
   Serial.println("RPM: ");
   Serial.println(rpm);
   Serial.println("Acceleration in mph^2: ");
   Serial.println(acceleration_mph);
   
 }
