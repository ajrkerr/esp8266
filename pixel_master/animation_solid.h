#ifndef ANIMATION_SOLID_H
#define ANIMATION_SOLID_H

#include "animation.h"
#include "pixel_strip.h"
#include "pixel_strip_config.h"

class AnimationSolid : public Animation {
public:

  void setup(unsigned long currentFrame, PixelStripConfig config, PixelStrip *strip) {
    Pixel pixel;

    for(int i = 0; i < config.numPixels; i++) { 
      pixel.red = (int)config.primaryColor.red;
      pixel.green = (int)config.primaryColor.green;
      pixel.blue = (int)config.primaryColor.blue;

      strip->setPixel(i, pixel);
    }
  }

  void loop(unsigned long currentFrame, PixelStripConfig config, PixelStrip *strip) {}
};

#endif