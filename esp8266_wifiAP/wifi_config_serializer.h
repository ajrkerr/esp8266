#ifndef WIFI_CONFIG_SERIALIZER_H
#define WIFI_CONFIG_SERIALIZER_H

#include <Arduino.h>
#include "wifi_config.h"
#include "json.h"

class WifiConfigSerializerClass {
public:
  String toJSON(WifiConfig *wifiConfig);
};

extern WifiConfigSerializerClass WifiConfigSerializer;

#endif