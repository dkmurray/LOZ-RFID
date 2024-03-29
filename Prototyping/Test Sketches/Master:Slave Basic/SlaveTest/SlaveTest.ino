#include <pcmRF.h>
#include <pcmConfig.h>
#include <TMRpcm.h>

#include "SdFat.h"
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

TMRpcm tmrpcm;
#define SD_ChipSelectPin 4

void setup() {
  tmrpcm.speakerPin = 9;
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }
}

void loop() {
  delay(100);
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
  if(x%15==0){
    tmrpcm.setVolume(5);
    tmrpcm.play("test.wav");
  }
}