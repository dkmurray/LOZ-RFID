#include "TMRpcm.h"
#include "SPI.h"

// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

TMRpcm audio;
#define SD_ChipSelectPin 8
#define led1 6
#define led2 7

void setup() {
  audio.speakerPin = 9;
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }
}

void loop() {
  audio.setVolume(5);
  audio.play("twilight.wav");
  delay(1800000);
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
  if(x==200){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    audio.setVolume(5);
    audio.play("wolf2.wav");
    delay(5000);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    audio.play("twilight.wav");
  }
}
