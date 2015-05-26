#ifndef PIXEL_DRIVER_H
#define PIXEL_DRIVER_H

#include "pixel_strip.h"
#include "pixel_strip_config.h"
#include "pixel.h"
#include <RGBConverter.h>

class PixelDriver {
  PixelStripConfig config;
  
  unsigned long currentFrame;
  PixelStrip *strip;
  double *flameDirection;
  double *flameDelta;

public: 
  void setup(PixelStripConfig *newConfig);
  void setConfig(PixelStripConfig *newConfig);
  void setPixel(int position, Pixel pixel);
  Pixel getPixel(int position);
  void loop();

private:
  void drawFrame();
};

#endif