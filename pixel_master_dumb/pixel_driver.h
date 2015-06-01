#ifndef PIXEL_DRIVER_H
#define PIXEL_DRIVER_H

#include "pixel_strip.h"
#include "pixel_strip_config.h"
#include "pixel.h"

#include "animation_solid.h"
#include "animation_tracer.h"
#include "animation_rainbow.h"
#include "animation_flame.h"

class PixelDriver {
  PixelStripConfig config;
  
  unsigned long currentFrame;
  PixelStrip *strip;
  Animation *animation;

public: 
  void setup(PixelStripConfig aConfig);
  void setConfig(PixelStripConfig newConfig);
  void setPixel(int position, Pixel pixel);
  Pixel getPixel(int position);
  void loop();

private:
  void drawFrame();
};

#endif