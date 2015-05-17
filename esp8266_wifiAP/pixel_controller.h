#ifndef PIXEL_CONTROLLER_H
#define PIXEL_CONTROLLER_H

#include "pixel_config.h"

class PixelController {
  const char *preamble = "12345"; //arbitrary
  const char *postamble = "54321"; //arbitrary

  HardwareSerial *serial;

public:
  PixelController() {
    serial = &Serial;
  }

  PixelController(HardwareSerial *hardwareSerial) {
    serial = hardwareSerial;
  }

  void send(PixelConfig *config) {
    char *buffer = (char*)config;
    
    serial->write(preamble, sizeof(preamble));
    serial->write(buffer, sizeof(*config));
    serial->write(postamble, sizeof(postamble));
  }
};

#endif
