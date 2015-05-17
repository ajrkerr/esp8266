#ifndef PIXEL_CONTROLLER_H
#define PIXEL_CONTROLLER_H

#include "pixel_config.h"
#include "checksum.h"

class PixelController {
  const char *preamble = "123"; //arbitrary
  const char *postamble = "321"; //arbitrary

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
    
    serial->write(preamble, strlen(preamble));
    serial->write(buffer, sizeof(*config));
    serial->write(Checksum.calculate(buffer, sizeof(*config)));
    serial->write(postamble, strlen(postamble));
  }
};

#endif
