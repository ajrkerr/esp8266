#ifndef PIXEL_CONTROLLER_H
#define PIXEL_CONTROLLER_H

#include "pixel_config.h"

class PixelControllerClass {
public:
  void send(PixelConfig *config) {
    char *buffer = (char*)config;
    
    Serial.write(buffer, sizeof(*config));
  }
};

extern PixelControllerClass PixelController;

#endif
