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
  Pixel pixel;

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
    
    double primaryHSV[3];
    conv.rgbToHsv(config.primaryColor.red, config.primaryColor.green, config.primaryColor.blue, primaryHSV);
    double primaryHue = primaryHSV[0];
    double primarySaturation = primaryHSV[1];
    double primaryValue = primaryHSV[2];

    double maxVariance = 0.08;
    double flameStep = 0.00085;
    int entropy = 07;

    byte rgb[3];

    for(int i = 0; i < config.numPixels; i++) { 
      if(random(100) < entropy) {
        flameDirection[i] = flameDirection[i] * -1;
      }

      flameDelta[i] += flameDirection[i] * flameStep;

      if(flameDelta[i] > maxVariance) {
        flameDirection[i] = -1;
        flameDelta[i] = maxVariance;
      } else if((0 - flameDelta[i]) < (0 - maxVariance)) {
        flameDirection[i] = 1;
        flameDelta[i] = 0 - maxVariance;
      }

      double hue = constrain(primaryHue + flameDelta[i], 0.0, 1.0);
      double saturation = primarySaturation;
      double value = constrain(primaryValue + flameDelta[i], 0.0, 1.0);

      conv.hsvToRgb(hue, saturation, value, rgb);
      pixel.red = rgb[0];
      pixel.green = rgb[1];
      pixel.blue = rgb[2];

      strip->setPixel(i, pixel);
    }
  }
};

#endif