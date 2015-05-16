#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 8
#define DATA_PIN 9
#define DELAY 500

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
}

unsigned long frameNum = 0;
void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() > 0) {
    Serial.write(Serial2.read());
  } 
  
  if(frameNum * DELAY < micros()) {
    CRGB color = (frameNum % 2 == 0)? CRGB::Blue : CRGB::Black;
    
    leds[0] = color;

    FastLED.show();
    frameNum++;
  }
}