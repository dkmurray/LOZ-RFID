#include <pcmRF.h>
#include <pcmConfig.h>
#include <TMRpcm.h>

#include "SdFat.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;

void setup(){
tmrpcm.speakerPin = 9;
Serial.begin(9600);
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;
}

tmrpcm.setVolume(5);
tmrpcm.play("test.wav");
}

void loop(){ }