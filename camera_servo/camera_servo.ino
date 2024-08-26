/*
  wire receiver

  > arduino-cli board list
  Port                            Protocol Type              Board Name                FQBN             Core
  /dev/cu.usbmodem2201            serial   Serial Port (USB) Arduino Mega or Mega 2560 arduino:avr:mega arduino:avr

  cd src/garden-rov-arduino/camera_servo
  arduino-cli compile -e --fqbn arduino:avr:mega .
  arduino-cli upload -p /dev/cu.usbmodem2201 --fqbn arduino:avr:mega .
  arduino-cli monitor -p /dev/cu.usbmodem2201 -b arduino:avr:mega

*/


#include <Wire.h>

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop()
{
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  char pbuff[255] = "";
  sprintf(pbuff, "in receiveEvent, howMany: %d\n", howMany);
  Serial.println(pbuff);
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);      // print the character
  }
  int xx = Wire.read();    // receive byte as an integer
  sprintf(pbuff, "byte as byte: %x\n", xx);
  Serial.println(pbuff);      // print the integer
}
