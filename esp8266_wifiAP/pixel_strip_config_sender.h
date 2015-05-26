#ifndef PIXEL_STRIP_CONFIG_SENDER_H
#define PIXEL_STRIP_CONFIG_SENDER_H

#include "pixel_strip_config.h"
#include "checksum.h"

class PixelStripConfigSender {
  const char *preamble = "12"; //arbitrary
  const char *postamble = "21"; //arbitrary

public:
  void send(PixelStripConfig config) {
    char *buffer = (char*)&config;
    
    Serial.write(preamble, strlen(preamble));
    Serial.write(buffer, sizeof(PixelStripConfig));
    Serial.write(Checksum.calculate(buffer, sizeof(PixelStripConfig)));
    Serial.write(postamble, strlen(postamble));
  }
};

#endif
