#ifndef ANIMATION_FLAME_H
#define ANIMATION_FLAME_H

#include "animation.h"
#include "pixel_strip.h"
#include "pixel_strip_config.h"
#include <RGBConverter.h>

class AnimationFlame : public Animation {
  RGBConverter conv;
  double *flameDirection;
  double *flameDelta;

public:
  ~AnimationFlame() {
    free(flameDirection);
    free(flameDelta);
  }

  void setup(unsigned long currentFrame, PixelStripConfig config, PixelStrip *strip) {
    flameDirection = (double*) malloc(sizeof(double) * config.numPixels);
    flameDelta = (double*) malloc(sizeof(double) * config.numPixels);

    for(int i = 0; i < config.numPixels; i++) {
      flameDirection[i] = 1.0;
      flameDelta[i] = 0.0;
    }

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