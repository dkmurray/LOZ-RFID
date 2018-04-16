#include "TMRpcm.h"
#include "SPI.h"
#include <Wire.h>
#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    25
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

TMRpcm audio;
#define SD_ChipSelectPin 8

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
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
    redLEDS(true);
    audio.setVolume(5);
    audio.play("secret.wav");
    delay(1500);
    audio.pause();
    delay(1500);
    redLEDS(false);
  }
}

void redLEDS(boolean on){
  for(int i; i < NUM_LEDS; i++){
    if(on){
      leds[i] = CRGB::Red;
    }
    else{
      leds[i] = CRGB::Black;
    }
    FastLED.show();
  }
}
