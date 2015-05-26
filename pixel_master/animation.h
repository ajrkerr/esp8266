#ifndef ANIMATION_H
#define ANIMATION_H

#include "pixel_strip.h"
#include "pixel_strip_config.h"

class Animation {
public:
  virtual void setup(unsigned long currentFrame, PixelStripConfig config, PixelStrip *strip);
  virtual void loop(unsigned long currentFrame, PixelStripConfig config, PixelStrip *strip);
};

#endif