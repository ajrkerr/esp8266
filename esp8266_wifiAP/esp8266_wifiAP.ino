#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define MAX_WIFI_ATTEMPTS 20
#define SERIAL_BAUD 9600
#define DEBUG 0

#include "wifi_wrapper.h"
#include "pixel_config.h"
#include "http_controller.h"
#include "wifi_config_repository.h"

WifiWrapper wifiWrapper;
PixelConfig pixelConfig;
HttpController httpController(&wifiWrapper, &pixelConfig);
WifiConfig wifiConfig;

void setup() {
  Serial.begin(SERIAL_BAUD);
  
  setupWifi();
  
  PixelConfigRepository.load(&pixelConfig);
  httpController.setup();
  httpController.setWifiConfig(&wifiConfig);
}

void setupWifi() {
  loadWifiConfig();
  wifiWrapper.setup(&wifiConfig);

  if(!wifiWrapper.isConnected()) {
    useDefaultWifiConfig();
    wifiWrapper.setup(&wifiConfig);
  }
}

void loadWifiConfig() {
  if(!WifiConfigRepository.load(&wifiConfig)) {
    useDefaultWifiConfig();
  }
}

void useDefaultWifiConfig() {
  DEBUG_PRINTLN("Using default config");
  strncpy(wifiConfig.ssid, "ConfigureESP8266", sizeof(wifiConfig.ssid));
  strncpy(wifiConfig.password, "", sizeof(wifiConfig.password));
  strncpy(wifiConfig.hostname, "esp8266", sizeof(wifiConfig.hostname));
  wifiConfig.access_point = true;
}

void loop() {
  wifiWrapper.loop();
  httpController.loop();
}
