#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define MAX_WIFI_ATTEMPTS 20
#define SERIAL_BAUD 115200
#define DEBUG 0

#include "ESPAutoConf.h"
#include "pixel_config.h"
#include "http_controller.h"

ESPAutoConf autoConf;
PixelConfig pixelConfig;
HttpController httpController(&autoConf, &pixelConfig);
WifiConfig wifiConfig;

void setup() {
  Serial.begin(SERIAL_BAUD);
  
  setup_wifi();
  
  PixelConfigRepository.load(&pixelConfig);
  httpController.setup();
}

void setup_wifi() {
  loadWifiConfig();
  autoConf.setup(&wifiConfig);

  if(!autoConf.isConnected()) {
    useDefaultWifiConfig();
    autoConf.setup(&wifiConfig);
  }
}

void loadWifiConfig() {
  if(!WifiConfigRepository.load(&wifiConfig)) {
    useDefaultWifiConfig();
  }
}

void useDefaultWifiConfig() {
  strncpy(wifiConfig.ssid, "ConfigureESP8266", sizeof(wifiConfig.ssid));
  strncpy(wifiConfig.password, "", sizeof(wifiConfig.password));
  strncpy(wifiConfig.hostname, "esp8266", sizeof(wifiConfig.hostname));
  wifiConfig.access_point = true;
}

void loop() {
  autoConf.loop();
  httpController.loop();
}