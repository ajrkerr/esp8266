#ifndef PIXEL_CONTROLLER_H
#define PIXEL_CONTROLLER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "pixel_config.h"
#include <RGBConverter.h>

typedef struct {
  char red;
  char green;
  char blue;
} Pixel;

class PixelDriver {
  PixelConfig config;
  
  unsigned long currentFrame;

  double *flameDirection;
  double *flameDelta;
  Adafruit_NeoPixel *strip;

public: 
  void setup(PixelConfig *newConfig);
  void setConfig(PixelConfig *newConfig);
  void setPixel(int position, Pixel pixel);
  Pixel getPixel(int position);
  void loop();

private:
  void drawFrame();
};

#endif