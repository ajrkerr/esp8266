#ifndef PAGE_BUILDER_H
#define PAGE_BUILDER_H

#include <Arduino.h>

#include "wifi_config.h"
#include "pixel_config.h"
#include "WiFiClient.h"

class PageBuilder {

public:
  void html(WiFiClient client);
  void script(WiFiClient client);
  void bootstrap(WiFiClient client);

private:
  void write(WiFiClient client, const char buffer[], int length);
};

#endif