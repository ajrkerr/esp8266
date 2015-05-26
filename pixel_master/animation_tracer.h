#ifndef ANIMATION_TRACER_H
#define ANIMATION_TRACER_H

#include "animation.h"
#include "pixel_strip.h"
#include "pixel_strip_config.h"

class AnimationTracer : public Animation {
public:

  void setup(unsigned long currentFrame, PixelStripConfig config, PixelStrip *strip) {
    loop(currentFrame, config, strip);
  }

  void loop(unsigned long currentFrame, PixelStripConfig config, PixelStrip *strip) {
    Pixel pixel;
    
    for(int i = 0; i < config.numPixels; i++) { 
      if(currentFrame % config.numPixels == i) {
        pixel.red = (int)config.primaryColor.red;
        pixel.green = (int)config.primaryColor.green;
        pixel.blue = (int)config.primaryColor.blue;
      } else {
        pixel.red = (int)config.secondaryColor.red;
        pixel.green = (int)config.secondaryColor.green;
        pixel.blue = (int)config.secondaryColor.blue;
      }

      strip->setPixel(i, pixel);
    }
  }
};

#endif