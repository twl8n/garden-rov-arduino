// todo
// set turning, forward, backing flags
// decrease turns to straight forward or back
// turning use different min speed, maybe different speed decay

// Continuous drive strategy (vs V2 which was "drive 500 ms, stop motors, await next command".
// Continue driving, maybe forward, maybe slowly revert to forward or slow down?
// Buffer commands and support the cursor keys.

// Emacs, open buggyv3.ino
// arduino-cli compile --fqbn arduino:avr:uno .
// terminal 1:
// cd src/Arduino/buggyv3/build/arduino.avr.uno
// scp buggyv3.ino.hex raspberrypi.local:
// terminal 2:
// ssh raspberrypi.local
// arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno -i buggyv3.ino.hex
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

// Directional flags, adjust return to straigh, and speed decay min.
int is_turning = 0;
int is_forward = 0;
int is_backing = 0;

// When turning, the min speed should be closer to 16.
int min_speed = 10; // below this, we just go to zero.
int turning_min_speed = 14;

// Large numbers, like 20, decay too slow.
int speed_decay = 10; // speed = speed - (speed / speed_decay);

int speedx = 10; // speed = speed + (speed / speedx)
int l_motor_speed = 0; // motor 1 is left
int r_motor_speed = 0; // motor 2 is right

// These won't work because the shifted versions are only lefty and righty.
// Would need shift-left and control left, which isn't too obvious.
// And the shift-up and control-up have some other meaning, like speed.

// The numeric keypad in number mode works better, and simpler too.

char UP_ARROW[4] = "\033\133\101";
char LEFT_ARROW[4] = "\033\133\104";
char DOWN_ARROW[4] = "\033\133\102";
char RIGHT_ARROW[4] = "\033\133\103";

char SHIFT_UP_ARROW[7] = "\033\133\061\073\062\101";
char SHIFT_LEFT_ARROW[7] = "\033\133\061\073\062\104";
char SHIFT_DOWN_ARROW[7] = "\033\133\061\073\062\102";
char SHIFT_RIGHT_ARROW[7] = "\033\133\061\073\062\103";

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
  l_motor_speed = 0;
  r_motor_speed = 0; 
  int hit_ok = 1; // default to getting a hit
  if (sdir == "left")
    {
      l_motor_speed = -32;
      r_motor_speed = 32; 
    }
  else if (sdir == "slight_f_left")
    {
      l_motor_speed = -16;
      r_motor_speed = 32;
    }
  else if (sdir == "slight_b_left")
    {
      l_motor_speed = -32;
      r_motor_speed = 16;
    }
  else if (sdir == "right")
    {
      l_motor_speed = 32;
      r_motor_speed = -32;
    }
  else if (sdir == "slight_f_right")
    {
      l_motor_speed = 32;
      r_motor_speed = -16;
      // motor1.setSpeed(32);  // Motor 1 left forward
      // motor2.setSpeed(-16); // Motor 2 right back
    }
  else if (sdir == "slight_b_right")
    {
      l_motor_speed = 16;
      r_motor_speed = -32;
      // motor1.setSpeed(16);  // Motor 1 left forward
      // motor2.setSpeed(-32); // Motor 2 right back
    }
  else if (sdir == "forward")
    {
      l_motor_speed = 16;
      r_motor_speed = 16;
      // motor1.setSpeed(16);  // Motor 1 forward
      //motor2.setSpeed(16);  // Motor 2 forward
    }
  else if (sdir == "backward")
    {
      l_motor_speed = -16;
      r_motor_speed = -16;
      // motor1.setSpeed(-16);  // Motor 1 back
      // motor2.setSpeed(-16);  // Motor 2 back
    }
  else if (sdir == "stop")
    {
      l_motor_speed = 0;
      r_motor_speed = 0;
    }
  else hit_ok = 0;
  
 if (hit_ok)
    {
      // compensate based on the speedx multiplier
      l_motor_speed = l_motor_speed + (l_motor_speed / speedx);
      r_motor_speed = r_motor_speed + (r_motor_speed / speedx);
      motor1.setSpeed(l_motor_speed);
      motor2.setSpeed(r_motor_speed);
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

    if (0) // disable printing for now
      {
        sprintf(pbuff, "bytes_read %d\n\r",bytes_read);
        Serial.println(pbuff);
        for(int xx = 0; xx<strlen(inbuff); xx++)
          {
            sprintf(pbuff, "input[%d] %o\n\r", xx, inbuff[xx]);
            Serial.println(pbuff);
          }
      }
    
    //  would be stop, if we decide to support it.
    
    if (inbuff[0] == 'j' || inbuff[0] == '4' || strstr(inbuff, LEFT_ARROW) != NULL)
      {
        buggy_move("left");
      }
    else if (inbuff[0] == 'u' || inbuff[0] == '7' || strstr(inbuff, SHIFT_LEFT_ARROW) != NULL)
      {
        buggy_move("slight_f_left");
      }
    else if (inbuff[0] == 'n' || inbuff[0] == '1')
      {
        buggy_move("slight_b_left");
      }
    else if (inbuff[0] == 'k'|| inbuff[0] == '6')
      {
        buggy_move("right");
      }
    else if (inbuff[0] == 'o' || inbuff[0] == '9')
      {
        buggy_move("slight_f_right");
      }
    else if (inbuff[0] == ','|| inbuff[0] == '3')
      {
        buggy_move("slight_b_right");
      }
    else if (inbuff[0] == 'i' || inbuff[0] == '8' || strstr(inbuff, UP_ARROW) != NULL)
      {
        buggy_move("forward");
      }
    else if (inbuff[0] == 'm'|| inbuff[0] == '2')
      {
        buggy_move("backward");
      }
    else if (inbuff[0] == ' '|| inbuff[0] == '5')
      {
        buggy_move("stop");
      }
  }
  delay(250);
  // slow down over time
  if (l_motor_speed != 0) l_motor_speed = l_motor_speed - (l_motor_speed / speed_decay);
  if (abs(l_motor_speed) < min_speed) l_motor_speed = 0;
  if (r_motor_speed != 0) r_motor_speed = r_motor_speed - (r_motor_speed / speed_decay);
  if (abs(r_motor_speed) < min_speed) r_motor_speed = 0;
  // set the new speed
  motor1.setSpeed(l_motor_speed);
  motor2.setSpeed(r_motor_speed);
}
