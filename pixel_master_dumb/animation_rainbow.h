#ifndef ANIMATION_RAINBOW_H
#define ANIMATION_RAINBOW_H

#include "animation.h"
#include "pixel_strip.h"
#include "pixel_strip_config.h"
#include <RGBConverter.h>

class AnimationRainbow : public Animation {
public:

  void setup(unsigned long currentFrame, PixelStripConfig config, PixelStrip *strip) {
    loop(currentFrame, config, strip);
  }

  void loop(unsigned long currentFrame, PixelStripConfig config, PixelStrip *strip) {
    Pixel pixel;
    
    double hue;
    double sat = 1.0;
    double val = 1.0; 
  
    RGBConverter conv; // Convert from HSV to RGB space
    byte rgb[3];
  
    for(int i = 0; i < config.numPixels; i++) {
      int colorPos = i + currentFrame;
      int framesForCycle = 250;
      hue = ((double)(colorPos % framesForCycle) / framesForCycle);

      conv.hsvToRgb(hue, sat, val, rgb);
      
      pixel.red = rgb[0];
      pixel.green = rgb[1];
      pixel.blue = rgb[2];

      strip->setPixel(i, pixel);
    }
  }
};

#endif