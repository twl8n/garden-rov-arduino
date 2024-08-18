
// Compiling for and Arduino Uno
// cd path-to-ino-file
// arduino-cli compile --fqbn arduino:avr:uno .

/*
  This example shows how to drive 2 motors using the PWM and DIR pins with
  2-channel motor driver Cytron MDD10
  
  Jumpers in factory defaults
  PWM1 D9
  DIR1 D8
  
  PWM2 D11
  DIR2 D13
*/

#include "CytronMotorDriver.h"

// Configure the motor driver.
CytronMD motor1(PWM_DIR, 9, 8);  // Defaults jumpers 9,8
CytronMD motor2(PWM_DIR, 11, 13); // Default jumpers 11, 13


// Run the pattern once.
// Reset button on the Cytron shield is equivalent to the reset button on the Arduino, and easier to reach
// It will re-run this script.

// I'm using a 20V battery, so the motors rotational speed is faster than the same speed at 12V.

void setup() {
  motor1.setSpeed(16);   // Motor 1 forward at 6% speed. 16/255 = 6%
  motor2.setSpeed(16);   // Motor 2 forward at 6% speed.
  delay(2000);
  
  // stop
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  delay(1000);
  
  // turn left, requires more wheel speed than straight driving
  // I think motor1 is the left side.
  motor1.setSpeed(-48);  // backward 
  motor2.setSpeed(48);   // forward 
  delay(1000);

  // back 
  motor1.setSpeed(-16);  // back 6%
  motor2.setSpeed(-16);  
  delay(2000);

  // right
  motor1.setSpeed(48);  // Motor 1 forward
  motor2.setSpeed(-48); // Motor 2 back
  delay(1500);

  // back
  motor1.setSpeed(-16);  // back 6%
  motor2.setSpeed(-16);  
  delay(500);

  // left
  motor1.setSpeed(-48);  // Motor 1 back
  motor2.setSpeed(48);   // Motor 2 forward 
  delay(500);

  motor1.setSpeed(0);     // Motor 1 stops.
  motor2.setSpeed(0);     // Motor 2 stops.
  delay(1000);
  
}


void loop() {
}
