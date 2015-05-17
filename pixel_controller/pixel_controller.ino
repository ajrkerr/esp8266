#define SERIAL_BAUD 9600

#include "FastLED.h"
#include "pixel_config.h"
#include "serial_receiver.h"
#include "debug_utils.h"

PixelConfig pixelConfig;
SerialReceiver receiver(&Serial2);

#define NUM_LEDS 255
#define PIXEL_PIN 9

CRGB leds[NUM_LEDS];

void initConfig() {
  pixelConfig.numPixels = 4;
  pixelConfig.primaryColor.red = 255;
  pixelConfig.primaryColor.green = 0;
  pixelConfig.primaryColor.blue = 0;
}

void setup() { 
  Serial.begin(SERIAL_BAUD);
  Serial2.begin(SERIAL_BAUD);
  
  initConfig();
  refreshConfig();
}

void loop() { 
  receiver.loop();
  
  if(receiver.changed) {
    memcpy(&pixelConfig, &receiver.pixelConfig, sizeof(pixelConfig));
    refreshConfig();
  }

  updateLEDS();
}

void updateLEDS() {
  for(int i = 0; i < pixelConfig.numPixels; i++) { 
    leds[i].red = 255;
    leds[i].green = 0;
    leds[i].blue = 0;
  }

  FastLED.show();
}

void refreshConfig() {
  DEBUG_PRINTLN("New Config Received");
  DEBUG_PRINT("Primary Red: ");
  DEBUG_PRINTLN2(pixelConfig.primaryColor.red, DEC);
  DEBUG_PRINT("Primary Green: ");
  DEBUG_PRINTLN2(pixelConfig.primaryColor.green, DEC);
  DEBUG_PRINT("Primary Blue: ");
  DEBUG_PRINTLN2(pixelConfig.primaryColor.blue, DEC);

  for(int i = pixelConfig.numPixels; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }

  FastLED.addLeds<NEOPIXEL, PIXEL_PIN>(leds, NUM_LEDS);
  FastLED.show();

  FastLED.addLeds<NEOPIXEL, PIXEL_PIN>(leds, pixelConfig.numPixels); 
  FastLED.show();
}

