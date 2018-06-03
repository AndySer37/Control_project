// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <time.h>
char pi_data;
uint32_t piTimer;
void setup() {
  Wire.begin();                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  piTimer=millis();
}

void loop() {
    if ((millis() - piTimer) > 2000) {
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(millis());           // sends 16 bytes
    Serial.println(millis());
    Wire.requestFrom(9, 8);
    if (Wire.available()) {
      pi_data = Wire.read();
    }
    Wire.endTransmission();    // stop transmitting
//    control(pi_data);
    Serial.println(pi_data);
    piTimer=millis();
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}
