#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define MAX_WIFI_ATTEMPTS 20
#define SERIAL_BAUD 9600
#define DEBUG 0

#include "wifi_wrapper.h"
#include "pixel_strip_config.h"
#include "http_controller.h"
#include "wifi_config_repository.h"
#include "pixel_strip_config_sender.h"

WifiWrapper wifiWrapper;
PixelStripConfig pixelStripConfig;
HttpController httpController;
WifiConfig wifiConfig;
PixelStripConfigSender pixelStripConfigSender;

char buffer[6];

void setup() {
  Serial.begin(SERIAL_BAUD);

  setupWifi();

  PixelStripConfigRepository.load(&pixelStripConfig);
  httpController.setup(&wifiWrapper, &pixelStripConfigSender);
  httpController.wifiConfig = wifiConfig;
  httpController.pixelStripConfig = pixelStripConfig;
}

void setupWifi() {
  if (!WifiConfigRepository.load(&wifiConfig)) {
    useDefaultWifiConfig();
  }

  wifiWrapper.setup(&wifiConfig);

  if (!wifiWrapper.isConnected()) {
    useDefaultWifiConfig();
    wifiWrapper.setup(&wifiConfig);
  }
}

void useDefaultWifiConfig() {
  DEBUG_PRINTLN("Using default config");
  strncpy(wifiConfig.ssid, "ConfigureESP8266", sizeof(wifiConfig.ssid));
  strncpy(wifiConfig.password, "", sizeof(wifiConfig.password));
  strncpy(wifiConfig.hostname, "esp8266", sizeof(wifiConfig.hostname));
  wifiConfig.access_point = true;
}

void addToBuffer(char c) {
  for(int i = 0; i < 5; i++) {
    buffer[i] = buffer[i+1];
  }
  buffer[5] = c;
}

void loop() {
  wifiWrapper.loop();
  httpController.loop();

  if(Serial.available() > 0) {
    addToBuffer(Serial.read());

    if(strcmp(buffer, "RESEND") == 0) {
      pixelStripConfigSender.send(&httpController.pixelStripConfig);
    }
  }
}
