#include <FastLED.h>
#include <RGBConverter.h>

#include "pixel_strip_config.h"
#include "pixel_driver.h"

PixelDriver driver;
PixelStripConfig pixelStripConfig;

#define PIXEL_DRIVER_PIN 9

void buildConfig() {
  pixelStripConfig.numPixels = 120;
  pixelStripConfig.frameLength = 15;
  pixelStripConfig.type = PIXEL_FLAME;
  pixelStripConfig.primaryColor.red = 255;
  pixelStripConfig.primaryColor.green = 0;
  pixelStripConfig.primaryColor.blue  = 0;
}

void setup() { 
  buildConfig();
  driver.setup(pixelStripConfig);
}

void loop() { 
  driver.loop();
}
