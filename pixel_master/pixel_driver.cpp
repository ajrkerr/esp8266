#include "pixel_driver.h"
#include "debug_utils.h"
#define DATA_PIN 9

void PixelDriver::setup(PixelStripConfig aConfig) {
  DEBUG_PRINT("Building PixelDriver");
  strip = new WSPixelStrip(DATA_PIN, config.numPixels);
  animation = NULL;
  setConfig(aConfig);
}

void PixelDriver::setConfig(PixelStripConfig newConfig) {
  config = newConfig;

  strip->resize(config.numPixels);
  currentFrame = millis() / config.frameLength;

  if(animation != NULL) {
    free(animation);
    animation = NULL;
  }
  
  switch(config.type) {
    case PIXEL_SOLID:
      animation = new AnimationSolid();
      break;
    case PIXEL_TRACER:
      animation = new AnimationTracer();
      break;
    case PIXEL_RAINBOW:
      animation = new AnimationRainbow();
      break;
    case PIXEL_FLAME:
      animation = new AnimationFlame();
      break;
  }

  animation->setup(currentFrame, config, strip);
}

void PixelDriver::loop() {
  if((currentFrame * config.frameLength) < millis()) {
    currentFrame++;

    drawFrame();
    strip->draw();
  }
}

void PixelDriver::drawFrame() {
  animation->loop(currentFrame, config, strip);
}
