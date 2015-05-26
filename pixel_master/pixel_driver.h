#ifndef PIXEL_DRIVER_H
#define PIXEL_DRIVER_H

#include "pixel_strip.h"
#include "pixel_config.h"
#include "pixel.h"
#include <RGBConverter.h>

class PixelDriver {
  PixelConfig config;
  
  unsigned long currentFrame;
  PixelStrip *strip;
  double *flameDirection;
  double *flameDelta;

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