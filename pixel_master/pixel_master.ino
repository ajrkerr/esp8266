#define SERIAL_BAUD 9600

#include <FastLED.h>
#include <RGBConverter.h>

#include "pixel_strip_config.h"
#include "pixel_driver.h"
#include "serial_receiver.h"

SerialReceiver receiver(&Serial2);
PixelDriver driver;
PixelStripConfig pixelStripConfig;

#define PIXEL_DRIVER_PIN 9

void initConfig() {
  pixelStripConfig.numPixels = 120;
  pixelStripConfig.frameLength = 15;
  pixelStripConfig.type = PIXEL_FLAME;
  pixelStripConfig.primaryColor.red = 255;
  pixelStripConfig.primaryColor.green = 0;
  pixelStripConfig.primaryColor.blue  = 0;
}

void setup() { 
  Serial.begin(SERIAL_BAUD);
  Serial2.begin(SERIAL_BAUD);
  Serial2.println("RESEND");
  DEBUG_PRINT("Serials are setup");
  initConfig();
  DEBUG_PRINT("Config Built");
  driver.setup(pixelStripConfig);
}

void loop() { 
  receiver.loop();
  
  if(receiver.changed) {
    driver.setConfig(receiver.pixelStripConfig);
  }

  driver.loop();
}
