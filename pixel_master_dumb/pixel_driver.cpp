#include "pixel_driver.h"
#include "debug_utils.h"

void PixelDriver::setup(PixelStripConfig aConfig) {
  strip = new WSPixelStrip(PIXEL_DRIVER_PIN, aConfig.numPixels);
  animation = NULL;
  setConfig(aConfig);
}

void PixelDriver::setConfig(PixelStripConfig newConfig) {
  config = newConfig;

  strip->resize(config.numPixels);
  currentFrame = millis() / config.frameLength;

  if(animation != NULL) {
    delete animation;
    animation = NULL;
  }
  
  switch(config.type) {
    case PIXEL_SOLID:
      DEBUG_PRINT("Changing to solid");
      animation = new AnimationSolid();
      break;
    case PIXEL_TRACER:
      DEBUG_PRINT("Changing to Tracer");
      animation = new AnimationTracer();
      break;
    case PIXEL_RAINBOW:
      DEBUG_PRINT("Changing to rainbow");
      animation = new AnimationRainbow();
      break;
    case PIXEL_FLAME:
      DEBUG_PRINT("Changing to flame");
      animation = new AnimationFlame();
      break;
  }

  if(animation != NULL) {
    animation->setup(currentFrame, config, strip);
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
  animation->loop(currentFrame, config, strip);
}
