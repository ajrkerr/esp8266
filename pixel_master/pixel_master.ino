#define SERIAL_BAUD 9600

#include <Adafruit_NeoPixel.h>
#include <RGBConverter.h>

#include "pixel_config.h"
#include "pixel_driver.h"
#include "serial_receiver.h"

SerialReceiver receiver(&Serial2);
PixelDriver driver;
PixelConfig pixelConfig;

#define PIXEL_DRIVER_PIN 9

void initConfig() {
  pixelConfig.numPixels = 8;
  pixelConfig.frameLength = 15;
  pixelConfig.type = PIXEL_FLAME;
  pixelConfig.primaryColor.red = 229;
  pixelConfig.primaryColor.green = 38;
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
    debuConfig();
    driver.setConfig(&receiver.pixelConfig);
  }

  driver.loop();
}


void debuConfig() {
  DEBUG_PRINT("PIXEL_SOLID: ");
  DEBUG_PRINTLN2(PIXEL_SOLID , DEC);
  DEBUG_PRINT("PIXEL_TRACER: ");
  DEBUG_PRINTLN2(PIXEL_TRACER , DEC);
  DEBUG_PRINT("PIXEL_RAINBOW: ");
  DEBUG_PRINTLN2(PIXEL_RAINBOW, DEC);
  DEBUG_PRINT("PIXEL_FLAME: ");
  DEBUG_PRINTLN2(PIXEL_FLAME, DEC);
  DEBUG_PRINT("frameLength: ");
  DEBUG_PRINTLN2(receiver.pixelConfig.frameLength, DEC);
  DEBUG_PRINT("numPixels: ");
  DEBUG_PRINTLN2(receiver.pixelConfig.numPixels, DEC);
  DEBUG_PRINT("type: ");
  DEBUG_PRINTLN2(receiver.pixelConfig.type, DEC);
  DEBUG_PRINT("primaryColor.red: ");
  DEBUG_PRINTLN2(receiver.pixelConfig.primaryColor.red, DEC);
  DEBUG_PRINT("primaryColor.green: ");
  DEBUG_PRINTLN2(receiver.pixelConfig.primaryColor.green, DEC);
  DEBUG_PRINT("primaryColor.blue: ");
  DEBUG_PRINTLN2(receiver.pixelConfig.primaryColor.blue, DEC);
  DEBUG_PRINT("secondaryColor.red: ");
  DEBUG_PRINTLN2(receiver.pixelConfig.secondaryColor.red, DEC);
  DEBUG_PRINT("secondaryColor.green: ");
  DEBUG_PRINTLN2(receiver.pixelConfig.secondaryColor.green, DEC);
  DEBUG_PRINT("secondaryColor.blue: ");
  DEBUG_PRINTLN2(receiver.pixelConfig.secondaryColor.blue, DEC);
}
