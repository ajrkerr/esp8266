#ifndef PIXEL_CONTROLLER_H
#define PIXEL_CONTROLLER_H

#include "pixel_config.h"
#include "checksum.h"

class PixelController {
public:
  void send(PixelConfig *config) {
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
