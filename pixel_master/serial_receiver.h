#ifndef SERIAL_RECEIVER_H
#define SERIAL_RECEIVER_H

#include "pixel_config.h"
#include "debug_utils.h"
#include "checksum.h"
#include "buffer.h"
#include <Arduino.h>

class SerialReceiver {
  HardwareSerial *serial;
  Buffer *buffer;

  char *preamble = "123";

public:
  bool changed;
  PixelConfig pixelConfig;

  SerialReceiver(HardwareSerial *hardwareSerial);
  void loop();
};

#endif