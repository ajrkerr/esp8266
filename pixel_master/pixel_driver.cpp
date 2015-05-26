#include "pixel_driver.h"
#define DATA_PIN 9

void PixelDriver::setup(PixelConfig *newConfig) {
  setConfig(newConfig);
  strip = new WSPixelStrip(DATA_PIN, config.numPixels);
}

void PixelDriver::setConfig(PixelConfig *newConfig) {
  memcpy(&config, newConfig, sizeof(config));

  strip->resize(config.numPixels);
  currentFrame = millis() / config.frameLength;

  free(flameDirection);
  free(flameDelta);

  flameDirection = (double*) malloc(sizeof(double) * config.numPixels);
  flameDelta = (double*) malloc(sizeof(double) * config.numPixels);
  
  for(int i = 0; i < config.numPixels; i++) {
    flameDirection[i] = 1.0;
    flameDelta[i] = 0.0;
  }
}

void PixelDriver::loop() {
  if((currentFrame * config.frameLength) < millis()) {
    currentFrame++;

    drawFrame();
    strip->draw();
  }
}

void PixelDriver::drawFrame() {
  Pixel pixel;

  if(config.type == PIXEL_SOLID) {
    for(int i = 0; i < config.numPixels; i++) { 
      pixel.red = (int)config.primaryColor.red;
      pixel.green = (int)config.primaryColor.green;
      pixel.blue = (int)config.primaryColor.blue;

      strip->setPixel(i, pixel);
    }
  } else if(config.type == PIXEL_TRACER) {
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
  } else if(config.type == PIXEL_RAINBOW) {
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
  } else if(config.type == PIXEL_FLAME) {
    RGBConverter conv;
    
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
}
