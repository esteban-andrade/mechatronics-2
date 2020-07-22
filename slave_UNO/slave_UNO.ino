#include "SD.h"
#include "TMRpcm.h"
#include "Wire.h"

#define SD_ChipSelectPin 10


TMRpcm tmrpcm;

void setup() {
  Wire.begin(5);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  tmrpcm.speakerPin = 9;
  if (!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }
  tmrpcm.setVolume(5);
}

void loop() {
}

void receiveEvent() {
  if (tmrpcm.isPlaying() == 0) {
    if (Wire.available()) {
      char c = Wire.read();
      if (c == '0') {
        tmrpcm.play("wiiMusic.wav");
        while (tmrpcm.isPlaying()) {
          c = Wire.read();
          if (c == '2') {
            tmrpcm.stopPlayback();
            tmrpcm.play("walle.wav");  //ONCE THIS FILE FINISHES IT STARTS SEARCH
          }
        }
      } else if (c == '6') {
        tmrpcm.play("thugLyfe.wav");
        while (tmrpcm.isPlaying()) {
          c = Wire.read();
          if (c == '7') {
            tmrpcm.stopPlayback();
          }
        }
      } else if (c == '3') {
        tmrpcm.play("walle.wav");
      } else if (c == '5') {
        tmrpcm.play("wiiMusic.wav");
        while (tmrpcm.isPlaying()) {
          c = Wire.read();
          if (c == '7') {
            tmrpcm.stopPlayback();
          }
        }
      }
    }
  }
}


