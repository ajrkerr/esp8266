#ifndef HTTP_CONTROLLER_H
#define HTTP_CONTROLLER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "debug_utils.h"

#include "ESP8266WebServer.h"
#include "page_builder.h"

#include "wifi_config.h"
#include "wifi_wrapper.h"
#include "wifi_config_repository.h"

#include "pixel_controller.h"
#include "pixel_config.h"
#include "pixel_config_repository.h"


class HttpController {
  WifiConfig *wifiConfig;
  PixelConfig *pixelConfig;

  WifiWrapper *wifiWrapper;
  ESP8266WebServer httpServer;

  PixelController pixelController;
  PageBuilder pageBuilder;

public:
  void setup(WifiWrapper *newWifiWrapper, WifiConfig *newWifiConfig, PixelConfig *newPixelConfig);
  void loop();
  
  void setWifiConfig(const WifiConfig *newConfig);
  void setPixelConfig(const PixelConfig *newConfig);

private: 
  void setupPages();
};

#endif
