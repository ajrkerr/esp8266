#ifndef PAGE_BUILDER_H
#define PAGE_BUILDER_H

#include <Arduino.h>

#include "wifi_config.h"
#include "pixel_config.h"
#include "WiFiClient.h"

class PageBuilder {

public:
  String build(WifiConfig *wifiConfig, PixelConfig *pixelConfig, WiFiClient client);
};

#endif