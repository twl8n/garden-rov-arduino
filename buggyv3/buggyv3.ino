// todo
// set turning, forward, backing flags
// decrease turns to straight forward or back
// turning use different min speed, maybe different speed decay
// need dynamic speed multiplier
// is keypad 3 "turn the front right" or "back up to the right"? I think back+right.

// Continuous drive strategy (vs V2 which was "drive 500 ms, stop motors, await next command".
// Continue driving, maybe forward, maybe slowly revert to forward or slow down?
// Buffer commands and support the cursor keys.

// Emacs, open buggyv3.ino
// arduino-cli compile -e --fqbn arduino:avr:uno .
// terminal 1:
// cd src/Arduino/buggyv3/build/arduino.avr.uno
/*
  When directly connected (vs going through RPi)
  > arduino-cli board list
  Port                            Protocol Type              Board Name                FQBN             Core
  /dev/cu.usbmodem2101            serial   Serial Port (USB) Arduino Uno               arduino:avr:uno  arduino:avr
  arduino-cli upload -p /dev/cu.usbmodem2101 --fqbn arduino:avr:uno .
  arduino-cli monitor --raw -p /dev/cu.usbmodem2101 -b arduino:avr:uno

*/

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
#include <Servo.h>
#include <Wire.h>

byte xbyte = 0;


// Configure the motor driver.
CytronMD motor1(PWM_DIR, 9, 8);  // Defaults jumpers 9,8
CytronMD motor2(PWM_DIR, 11, 13); // Default jumpers 11, 13

Servo ud_servo; // up down servo
Servo lr_servo; // left right

int ud_last_pos = 90;
int lr_last_pos = 90;

size_t avail_chars = 0;
size_t bytes_read = 0;
char inbuff[255] = "";
char pbuff[255];

// Directional flags, adjust return to straigh, and speed decay min.
int is_turning = 0;
int is_forward = 0;
int is_backing = 0;

// When turning, the min speed should be closer to 16.
int min_speed = 20; // below this, we just go to zero.
int turning_min_speed = 14;

// Large numbers, like 20, decay too slow.
int speed_decay = 10; // speed = speed - (speed / speed_decay);

// not sure, but max pwm might be 255? With 20V we don't want to go over 153.
int max_speed = 96; 

// speedx 1 doubles the speed
int default_speedx = 1;
int speedx = default_speedx; // speed = speed + (speed / speedx)
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

// Run the pattern once.
// Reset on the Cytron shield is same as reset on the Arduino: re-run this script.
void setup() {
  Serial.begin(9600);
  Serial.println("\n\rStarting. j for left, k for right, i for forward, m for back\n\r");
  Serial.println("u i o\n\r");
  Serial.println(" j k \n\r");
  Serial.println("n m ,\n\r");

  
  /*
    Don't use any of the motor driver pins for other stuff: 8, 9, 11, 13
    Motor PWM: 9, 11
    Motor direction: 8, 13
    I guess "digitial" pins are TTL level: 2, 4, 7, 8, 12, 13
    The uno seems to have 6 PWM pins: 3, 5, 6, 9, 10, 11

    unused PWM pins: 3, 5, 6, 10
  
    https://docs.arduino.cc/tutorials/generic/secrets-of-arduino-pwm/
  
    OC0A	6
    OC0B	5
    OC1A	9
    OC1B	10
    OC2A	11
    OC2B	3

    https://forum.arduino.cc/t/simultaneous-pwm-to-several-pins/254952/2

    Doesn't seem that you can do analogWrites to pins 3, 5 & 6 in PORTD or 9, 10 &11 in PORTB. It's a bit
    setting thing, so you get highs and lows only.

    The PWM outputs generated on pins 5 and 6 will have higher-than-expected duty cycles. This is because of
    interactions with the millis() and delay() functions, which share the same internal timer used to generate
    those PWM outputs. This will be noticed mostly on low duty-cycle settings (e.g. 0 - 10) and may result in
    a value of 0 not fully turning off the output on pins 5 and 6.
  */

  // Enabling the servo pwm disables the left motor, motor 1
  // ud_servo.attach(6, 1000, 2000);
  motor1.setSpeed(0);     // Motor 1 stops.
  motor2.setSpeed(0);     // Motor 2 stops.

  Wire.begin(); // join i2c bus (address optional for master)
}

// Assume the calling code will turn the motors off.
void buggy_move(char *sdir)
{
  l_motor_speed = 0;
  r_motor_speed = 0; 
  int raws = 48;
  int hit_ok = 1; // default to getting a hit
  if (sdir == "left")
    {
      l_motor_speed = -(raws/1.1);
      r_motor_speed = raws/1.1; 
    }
  else if (sdir == "slight_f_left")
    {
      l_motor_speed = -(raws/1.5);
      r_motor_speed = raws;
    }
  else if (sdir == "right")
    {
        l_motor_speed = raws;
      r_motor_speed = -(raws);
    }
  else if (sdir == "slight_f_right")
    {
      l_motor_speed = raws;
      r_motor_speed = -(raws/1.5);
      // motor1.setSpeed(32);  // Motor 1 left forward
      // motor2.setSpeed(-16); // Motor 2 right back
    }
  else if (sdir == "slight_b_left")
    {
      l_motor_speed = raws/2.1; // 16;
      r_motor_speed = -(raws/2); //-32;
    }
  else if (sdir == "slight_b_right")
    {
      l_motor_speed = -(raws/2); // -24; // -32;
      r_motor_speed = raws/2.1; // 16;
      // motor1.setSpeed(16);  // Motor 1 left forward
      // motor2.setSpeed(-32); // Motor 2 right back
    }
  else if (sdir == "forward")
    {
      l_motor_speed = raws/2;
      r_motor_speed = raws/2;
      // motor1.setSpeed(16);  // Motor 1 forward
      //motor2.setSpeed(16);  // Motor 2 forward
    }
  else if (sdir == "backward")
    {
      l_motor_speed = -(raws/2);
      r_motor_speed = -(raws/2);
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
      if (l_motor_speed > 64) l_motor_speed = 64;
      if (r_motor_speed > 64) r_motor_speed = 64;
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
  if (avail_chars > 0) { // check for incoming serial data
    if (avail_chars > 255)
      {
        avail_chars = 255; // I wonder what happens to the other chars? Next loop maybe?
      }
    bytes_read = Serial.readBytes(inbuff, avail_chars);  // read all bytes, or max 255

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
    
    if (inbuff[0] == 'j' || inbuff[0] == '4')
      {
        buggy_move("left");
      }
    else if (inbuff[0] == 'u' || inbuff[0] == '7')
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
    else if (inbuff[0] == 'i' || inbuff[0] == '8')
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
    else if (inbuff[0] == 's')
      {
        // 2024-08-22 this isn't working.
        speedx = default_speedx;
        Serial.println("Enter a single digit:");
        char nums[] = "0123456789";
        String instr = Serial.readString();
        instr.toCharArray(inbuff, 255);
        char *num_ptr = strchr(nums, inbuff[0]);
        if (num_ptr != NULL)
          {
            int user_speed = atoi(inbuff[0]); // ?need to #include <stdlib.h>
            if (user_speed >= 0 && user_speed <= 3) speedx = 15;
            else if (user_speed > 2 && user_speed <= 7) speedx = 5;
            else if (user_speed > 7) speedx = 1;
          }
      }
    else if (strstr(inbuff, LEFT_ARROW) != NULL)
      {
        // camera servo left
      }
    else if (strstr(inbuff, RIGHT_ARROW) != NULL)
      {
        // camera servo left
      }
    else if (strstr(inbuff, DOWN_ARROW) != NULL)
      {
        // camera servo down, relative to server orientation
        if (ud_last_pos < 160)
          {
            ud_last_pos += 20;
          }
        else
          {
            ud_last_pos = 180;
          }
        sprintf(pbuff, "servo down %d\n\r", ud_last_pos);
        Serial.println(pbuff);
        ud_servo.write(ud_last_pos);
        // ud_servo.write(90);
      }
    else if (strstr(inbuff, UP_ARROW) != NULL)
      {
        // camera servo up, relative to servo orientation
        if (ud_last_pos > 20)
          {
            ud_last_pos -= 20;
          }
        else
          {
            ud_last_pos = 0;
          }
        sprintf(pbuff, "servo up %d\n\r", ud_last_pos);
        Serial.println(pbuff);
        ud_servo.write(ud_last_pos);
        // ud_servo.write(90);
      }
  }
  delay(250);
  // slow down over time
  if (l_motor_speed != 0) l_motor_speed = l_motor_speed - (l_motor_speed / speed_decay);
  if (abs(l_motor_speed) < min_speed) l_motor_speed = 0;
  if (r_motor_speed != 0) r_motor_speed = r_motor_speed - (r_motor_speed / speed_decay);
  if (abs(r_motor_speed) < min_speed) r_motor_speed = 0;
  // set the new speed
  if (l_motor_speed != 0 && r_motor_speed != 0)
    {
      sprintf(pbuff,"left: %d right: %d\n\r", l_motor_speed, r_motor_speed);
      Serial.println(pbuff);
    }
  motor1.setSpeed(l_motor_speed);
  motor2.setSpeed(r_motor_speed);

  if (xbyte < 25) {
    sprintf(pbuff, "writing to device 4: %x\n\r", xbyte);
    Serial.println(pbuff);
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write("x is ");       // sends five bytes
    Wire.write(xbyte);         // sends one byte
    Wire.endTransmission();    // stop transmitting
    
    sprintf(pbuff, "sizeof int: %d sizeof byte: %d sizeof string: %d\n\r", 
            sizeof(l_motor_speed),
            sizeof(xbyte),
            sizeof("x is "));
    Serial.println(pbuff);
    xbyte++;
    delay(500);
  }
}
