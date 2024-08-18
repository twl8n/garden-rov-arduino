
// Move the servo in a set pattern, then await commands on the serial port to move the sero

#include <Servo.h>

Servo myservo;              // create servo object to control a servo
                            // a maximum of eight servo objects can be created
int pos = 0;                // variable to store the servo position
unsigned long emillis = 1;  // don't assign this -1 or millis() - 1 since millis() might be zero resulting in underflow?
int phase = 4; // skip the initial stuff, now that we know it works.
char pbuff[255];
unsigned long curr_millis = millis();

void my_delay(unsigned long msec) {
  unsigned long milli_x = millis() + msec;
  char pbuff[255];
  while (milli_x > millis) {
    if ((millis() % 1000) == 0) {
      sprintf(pbuff, "%d %d", milli_x, millis());
      Serial.println(pbuff);
    }
  }
}

void setup() 
{
  Serial.begin(9600);
  myservo.attach(9, 1000, 2000);  // attaches the servo on pin 9 to the servo object
  // make sure there's a difference between millis() when we init above, and millis() when called below in
  // loop()
  delay(10);                      

  // A valiant but less than successful attempt to prevent servo bounce and position settling.
  long int scaling_factor = 65536;
  long int decay_factor = scaling_factor >> 1; // divide by 2
  long int sval = 180;
  for(int xx=0; xx<10; xx++) {
    // sval = ((sval * decay_factor ) >> 1);
    sval = sval >> 1;
    Serial.println(sval);
  }
  Serial.println("setup done");
}

void servo_move(char *sdir)
{
  Serial.println("servo_move");
  Serial.println(sdir);
  if (sdir == "left") {
    // servo.read() returns degrees, not microseconds
    for(int xx = myservo.read(); xx>0; xx--)
      {
        myservo.write(xx);
        delay(3);
      }
  } else if (sdir == "right") {
    // servo.read() returns degrees, not microseconds
    for(int xx = myservo.read(); xx<180; xx++)
      {
        myservo.write(xx);
        delay(3);
      }
  } else if (sdir == "center") {
    int servo_start = myservo.read(); // could be any value between 0 and 180
    for(int xx = servo_start; xx != 90; xx -= (xx > 90) - (xx < 90) )
      {
        myservo.write(xx);
        delay(3);
      }
  }
  // else what? wiggle?
}

size_t avail_chars = 0;
size_t bytes_read = 0;
char inbuff[255] = "";
int phase_pause = 1000;

void loop()
{
  if (phase == 4)
    {
      //
      // Must have the monitor in --raw to read without buffering until cr/lf.
      // arduino-cli monitor --raw -p /dev/cu.usbmodem2101 -b arduino:avr:uno
      // 
      avail_chars = Serial.available();
      if (avail_chars > 0) {                // check for incoming serial data
        // bytes_read = Serial.readBytes(inbuff, avail_chars);  // read all bytes
        // Serial.readBytes(inbuff, 1);  // read 1 bytes

        inbuff[0] = Serial.read();
        bytes_read = 1; // well, it is true.
        inbuff[1] = 0; // null terminate at [1], trim to length 1.

        sprintf(pbuff, "bytes_read %d input %s\n",bytes_read, inbuff);
        Serial.println(pbuff);

        if (inbuff[0] == 'l')
          {
            servo_move("left");
          }
        else if (inbuff[0] == 'r')
          {
            servo_move("right");
          }
        else if (inbuff[0] == 'c')
          {
            servo_move("center");
          }
      }
    }
  curr_millis = millis();
  if (((emillis) < curr_millis) && phase == 1)
    {
      Serial.println("Starting. l for left, r for right, empty for center");
      myservo.writeMicroseconds(1000);
      // myservo.write(0);
      sprintf(pbuff, "full ccw phase: %d curr_millis: %lu", phase, curr_millis);
      Serial.println(pbuff);
      phase++;
      emillis += phase_pause;
    }
  if ((emillis < curr_millis) && phase == 2)
    {
      myservo.writeMicroseconds(1500);
      //myservo.write(90);
      sprintf(pbuff, "center, phase: %d curr_millis: %lu", phase, curr_millis);
      Serial.println(pbuff);
      phase++;
      emillis += phase_pause;
    }
  if (emillis < curr_millis && phase == 3)
    {
      myservo.writeMicroseconds(2000);
      // myservo.write(180);
      sprintf(pbuff, "full cw, phase: %d currmillis: %lu\n\rdone.", phase, curr_millis);
      Serial.println(pbuff);
      phase++;
    }
}
