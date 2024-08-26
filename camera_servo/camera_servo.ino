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
#include <Servo.h>

Servo ud_servo; // up down servo
Servo lr_servo; // left right
int ud_pin = 2;
int lr_pin = 3;

// This must match buggyv3.ino
int client_address = 2; // The Wire lib casts this to uint8_t

int ud_last_pos = 90;
int lr_last_pos = 90;
char cin = 'x';
char pbuff[255] = "";

void setup()
{
  Wire.begin(client_address);   // join i2c bus as a client
  Wire.onReceive(receiveEvent); // register event handler function
  Serial.begin(9600);           // start serial for output

  ud_servo.attach(ud_pin, 1000, 2000);
  lr_servo.attach(lr_pin, 1000, 2000);
  ud_servo.write(90);
  lr_servo.write(90);
}

void loop()
{
  delay(100); // do we need this delay?
}

void old_receiveEvent(int howMany)
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

void servo_move(char smsg)
{
  int smin = 0;
  int smax = 180;
  int sincrement = 10;
  if (smsg == 'u') {
    // camera servo up, relative to servo orientation
    if (ud_last_pos > (smin + sincrement))
      {
        ud_last_pos -= sincrement;
      }
    else
      {
        ud_last_pos = smin;
      }
  } else if (smsg == 'd') {
    // camera servo down, relative to server orientation
    if (ud_last_pos < (smax - sincrement))
      {
        ud_last_pos += sincrement;
      }
    else
      {
        ud_last_pos = smax;
      }
  } else if (smsg == 'l') {
    // camera servo left, relative to server orientation
    if (lr_last_pos < (smax + sincrement))
      {
        lr_last_pos += sincrement;
      }
    else
      {
        lr_last_pos = smax;
      }
  } else if (smsg == 'r') {
    // camera servo left, relative to server orientation
    if (lr_last_pos > (smin + sincrement))
      {
        lr_last_pos -= sincrement;
      }
    else
      {
        lr_last_pos = smin;
      }
  }
  sprintf(pbuff, "smsg: %d %c ud servo %d lr servo: %d\n", (int)smsg, (char)smsg, ud_last_pos, lr_last_pos);
  Serial.print(pbuff);
  ud_servo.write(ud_last_pos);
  lr_servo.write(lr_last_pos);
}

/*
  Apparently not mentioned in the docs: the I2C internal buffer is 32 bytes.
*/

void receiveEvent(int howMany)
{
  cin = Wire.read();        // receive byte as a character
  // sprintf(pbuff, "howMany: %d cin: %x cin: %c\n", howMany, cin, (char)cin);
  // Serial.print(pbuff);
  servo_move(cin);
}
