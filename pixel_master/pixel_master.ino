#define SERIAL_BAUD 9600

#include <FastLED.h>
#include <RGBConverter.h>

#include "pixel_config.h"
#include "pixel_driver.h"
#include "serial_receiver.h"

SerialReceiver receiver(&Serial2);
PixelDriver driver;
PixelConfig pixelConfig;

#define PIXEL_DRIVER_PIN 9

void initConfig() {
  pixelConfig.numPixels = 120;
  pixelConfig.frameLength = 15;
  pixelConfig.type = PIXEL_FLAME;
  pixelConfig.primaryColor.red = 255;
  pixelConfig.primaryColor.green = 0;
  pixelConfig.primaryColor.blue  = 0;
}

void setup() { 
  Serial.begin(SERIAL_BAUD);
  Serial2.begin(SERIAL_BAUD);
  Serial2.println("RESEND");
  
  initConfig();
  driver.setup(&pixelConfig);
}

void loop() { 
  receiver.loop();
  
  if(receiver.changed) {
    driver.setConfig(&receiver.pixelConfig);
  }

  driver.loop();
}
