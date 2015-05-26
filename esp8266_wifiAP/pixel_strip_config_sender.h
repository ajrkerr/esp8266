#ifndef PIXEL_STRIP_CONFIG_SENDER_H
#define PIXEL_STRIP_CONFIG_SENDER_H

#include "pixel_strip_config.h"
#include "checksum.h"

class PixelStripConfigSender {
public:
  void send(PixelStripConfig *config) {
    const char *preamble = "123"; //arbitrary
    const char *postamble = "321"; //arbitrary

    char *buffer = (char*)config;
    
    Serial.write(preamble, strlen(preamble));
    Serial.write(buffer, sizeof(*config));
    Serial.write(Checksum.calculate(buffer, sizeof(*config)));
    Serial.write(postamble, strlen(postamble));
  }
};

#endif
