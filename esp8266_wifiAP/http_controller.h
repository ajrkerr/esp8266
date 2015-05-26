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

#include "pixel_strip_config_sender.h"
#include "pixel_strip_config.h"
#include "pixel_strip_config_repository.h"

#include "wifi_config_serializer.h"
#include "pixel_strip_config_serializer.h"


class HttpController {
public:
  WifiConfig wifiConfig;
  PixelStripConfig pixelStripConfig;

private:
  ESP8266WebServer httpServer;

  WifiWrapper *wifiWrapper;
  PixelStripConfigSender *pixelStripConfigSender;
  PageBuilder pageBuilder;

public:
  void setup(WifiWrapper *aWifiWrapper, PixelStripConfigSender *aPixelStripConfigSender);
  void loop();
  
private: 
  void setupPages();
};

#endif
