#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define MAX_WIFI_ATTEMPTS 20
#define SERIAL_BAUD 115200
#define DEBUG 0

#include "ESPAutoConf.h"
#include "http_controller.h"
#include "pixel_config.h"
 
ESPAutoConf autoConf;
PixelConfig pixelConfig;
HttpController httpController(&autoConf, &pixelConfig);

void setup() {
  Serial.begin(SERIAL_BAUD);
  autoConf.setup();
  PixelConfigRepository.load(&pixelConfig);
  httpController.setup();
}

void loop() {
  autoConf.loop();
  httpController.loop();
}
