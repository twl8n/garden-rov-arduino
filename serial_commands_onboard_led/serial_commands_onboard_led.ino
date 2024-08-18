// 2024 Tom Laudeman
//
// Run the arduino-cli monitor to send commands to the arduino. The commands are
// led_off
// led_on
// 
// Fixed a bug in most blink LED examples: the on board LED won't go off unless you set the pinMode of 13 to OUTPUT.

void setup() {
  Serial.begin(9600);  // start serial communication at 9600 baud
  pinMode(13, OUTPUT); // make it possible to turn off the onboard LED
}

void loop() {
  // Read and execute commands from serial port
  if (Serial.available()) {                // check for incoming serial data
    String command = Serial.readString();  // read a string, it will have a trailing newline
    command.trim();
    Serial.println(command == "led_off");
    if (command == "led_on") {
      digitalWrite(LED_BUILTIN, HIGH);  // turn on LED
    } else if (command == "led_off") {
      digitalWrite(LED_BUILTIN, LOW);  // turn off LED
    } else if (command == "read_a0") {
      Serial.println(analogRead(A0));  // read pin A0 and print
    }
  }
}
