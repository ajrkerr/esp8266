#ifndef SERIAL_RECEIVER_H
#define SERIAL_RECEIVER_H

#include "pixel_strip_config.h"
#include "debug_utils.h"
#include "checksum.h"
#include "buffer.h"
#include <Arduino.h>

class SerialReceiver {
  HardwareSerial *serial;
  Buffer *buffer;

  char *preamble = "12";

public:
  bool changed;
  PixelStripConfig pixelStripConfig;

  SerialReceiver(HardwareSerial *hardwareSerial);
  void loop();
};

#endif