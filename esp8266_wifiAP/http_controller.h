#ifndef HTTP_CONTROLLER_H
#define HTTP_CONTROLLER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "debug_utils.h"

#include "ESP8266WebServer.h"

#include "ESPAutoConf.h"
#include "pixel_controller.h"
#include "pixel_config.h"
#include "pixel_config_repository.h"


class HttpController {
  WifiConfig wifiConfig;
  ESPAutoConf *autoConf;
  ESP8266WebServer httpServer;
  PixelConfig *pixelConfig;
  PixelController controller;

public:
  HttpController(ESPAutoConf *newAutoConf, PixelConfig *newPixelConfig);

  void setup();
  void loop();

  String wifiConfigForm();
  String pixelConfigForm();
  
  void setupPages();

private:
  String header();
  String footer();
  String layout(String contents);
};

#endif
