
// Emacs, open buggyv2.ino
// arduino-cli compile --fqbn arduino:avr:uno .
// terminal 1:
// cd src/Arduino/buggyv2/build/arduino.avr.uno
// scp buggyv2.ino.hex raspberrypi.local:
// terminal 2:
// ssh raspberrypi.local
// twl@raspberrypi:~ $ arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno -i buggyv2.ino.hex
// arduino-cli monitor --raw -p /dev/ttyACM0 -b arduino:avr:uno
//
// MDD10 both err1 and err2 LEDs are on when USB plugged in: blown fuse in the power in to the MDD10 board
//
/* This example shows how to drive 2 motors using the PWM and DIR pins with
   2-channel motor driver.

   Jumpers in factory defaults
   PWM1 D9
   DIR1 D8

   PWM2 D11
   DIR2 D13
*/

#include "CytronMotorDriver.h"

size_t avail_chars = 0;
size_t bytes_read = 0;
char inbuff[255] = "";
char pbuff[255];
char UP_ARROW[3] = "\033\133\101";


// Configure the motor driver.
CytronMD motor1(PWM_DIR, 9, 8);  // Defaults jumpers 9,8
CytronMD motor2(PWM_DIR, 11, 13); // Default jumpers 11, 13


// Run the pattern once.
// Reset on the Cytron shield is same as reset on the Arduino: re-run this script.
void setup() {
  Serial.begin(9600);
  Serial.println("\n\rStarting. j for left, k for right, i for forward, m for back\n\r");
  Serial.println("u i o\n\r");
  Serial.println(" j k \n\r");
  Serial.println("n m ,\n\r");

  motor1.setSpeed(0);     // Motor 1 stops.
  motor2.setSpeed(0);     // Motor 2 stops.
}

// Assume the calling code will turn the motors off.
void buggy_move(char *sdir)
{
  if (sdir == "left")
    {
      motor1.setSpeed(-32);  // Motor 1 back
      motor2.setSpeed(32);   // Motor 2 forward 
    }
  else if (sdir == "slight_f_left")
    {
      motor1.setSpeed(-16);  // Motor 1 back
      motor2.setSpeed(32);   // Motor 2 forward 
    }
  else if (sdir == "slight_b_left")
    {
      motor1.setSpeed(-32);  // Motor 1 back
      motor2.setSpeed(16);   // Motor 2 forward 
    }
  else if (sdir == "right")
    {
      motor1.setSpeed(32);  // Motor 1 forward
      motor2.setSpeed(-32); // Motor 2 back
    }
  else if (sdir == "slight_f_right")
    {
      motor1.setSpeed(32);  // Motor 1 left forward
      motor2.setSpeed(-16); // Motor 2 right back
    }
  else if (sdir == "slight_b_right")
    {
      motor1.setSpeed(16);  // Motor 1 left forward
      motor2.setSpeed(-32); // Motor 2 right back
    }
  else if (sdir == "forward")
    {
      motor1.setSpeed(16);  // Motor 1 forward
      motor2.setSpeed(16);  // Motor 2 forward
    }
  else if (sdir == "backward")
    {
      motor1.setSpeed(-16);  // Motor 1 back
      motor2.setSpeed(-16);  // Motor 2 back
    }
}

// Serial.readBytes(inbuff, 1);  // read 1 bytes
// old read 1 char
// inbuff[0] = Serial.read();
// bytes_read = 1; // well, it is true.

void loop() {
  //
  // Must have the monitor in --raw to read without buffering until cr/lf.
  // arduino-cli monitor --raw -p /dev/cu.usbmodem2101 -b arduino:avr:uno
  // arduino-cli monitor --raw -p /dev/ttyACM0 -b arduino:avr:uno
  // 
  avail_chars = Serial.available();
  if (avail_chars > 0 && avail_chars < 255) {                // check for incoming serial data
    bytes_read = Serial.readBytes(inbuff, avail_chars);  // read all bytes

    // inbuff[1] = 0; // null terminate at [1], trim to length 1.
    inbuff[bytes_read] = 0; // null terminate at index bytes_read 

    sprintf(pbuff, "bytes_read %d\n\r",bytes_read);
    Serial.println(pbuff);
    for(int xx = 0; xx<strlen(inbuff); xx++)
      {
        sprintf(pbuff, "input[%d] %d\n\r", xx, inbuff[xx]);
        Serial.println(pbuff);
      }
    
    if (inbuff[0] == 'j')
      {
        buggy_move("left");
      }
    else if (inbuff[0] == 'u')
      {
        buggy_move("slight_f_left");
      }
    else if (inbuff[0] == 'n')
      {
        buggy_move("slight_b_left");
      }
    else if (inbuff[0] == 'k')
      {
        buggy_move("right");
      }
    else if (inbuff[0] == 'o')
      {
        buggy_move("slight_f_right");
      }
    else if (inbuff[0] == ',')
      {
        buggy_move("slight_b_right");
      }
    else if (inbuff[0] == 'i' || strstr(inbuff, UP_ARROW) != NULL)
      {
        buggy_move("forward");
      }
    else if (inbuff[0] == 'm')
      {
        buggy_move("backward");
      }
  }
  delay(500);
  motor1.setSpeed(0);     // Motor 1 stops.
  motor2.setSpeed(0);     // Motor 2 stops.
}
