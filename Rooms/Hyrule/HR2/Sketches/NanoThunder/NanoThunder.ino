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

#define relay4 7
#define relay3 6
#define relay2 5
#define relay1 4
#define led1 14
#define led2 17

void setup() {
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  audio.speakerPin = 9;
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
  if(x==200){
    flashEyes(true);
    flashStrips(true);
    audio.setVolume(5);
    audio.play("thunder.wav"); // 6 sec long
    delay(200);
    flashEyes(false);
    flashStrips(false);
    delay(75);
    flashEyes(true);
    flashStrips(true);
    delay(200);
    flashEyes(false);
    flashStrips(false);
    delay(100);
    flashEyes(true);
    flashStrips(true);
    delay(150);
    flashEyes(false);
    flashStrips(false);
    delay(75);
    flashEyes(true);
    flashStrips(true);
    delay(250);
    flashEyes(false);
    flashStrips(false);
    delay(50);
    flashEyes(true);
    flashStrips(true);
    delay(200);
    flashEyes(false);
    flashStrips(false);
    delay(100);
    flashEyes(true);
    flashStrips(true);
    delay(300);
    flashEyes(false);
    flashStrips(false);
    delay(200);
    flashEyes(true);
    flashStrips(true);
    delay(250);
    flashEyes(false);
    flashStrips(false);
    delay(50);
    flashEyes(true);
    flashStrips(true);
    delay(500);
    flashStrips(false);
    delay(3500);
    flashEyes(false);
  }
}

void flashEyes(bool on){
  if(on){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
  }
  else{
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
  }
}

void flashStrips(bool on){
  if(on){
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
  }
  else{
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
  }
}