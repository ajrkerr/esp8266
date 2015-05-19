#include "wifi_config_serializer.h"

String WifiConfigSerializerClass::toJSON(WifiConfig *wifiConfig) {
  JSONRoot *root = new JSONRoot();

  root
    ->string("ssid", wifiConfig->ssid)
    ->string("password", wifiConfig->password)
    ->string("hostname", wifiConfig->hostname)
    ->boolean("access_point", wifiConfig->access_point);

  return root->toString();
}

WifiConfigSerializerClass WifiConfigSerializer;