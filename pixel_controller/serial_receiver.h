#ifndef SERIAL_RECEIVER_H
#define SERIAL_RECEIVER_H

#include "pixel_config.h"
#include "debug_utils.h"
#include "checksum.h"
#include <Arduino.h>

class SerialReceiver {
  HardwareSerial *serial;
  int bufferSize;
  char *serialBuffer;

  PixelConfig tmpPixelConfig;

  char *preamble = "123";
  char *postamble = "321";
  
  void debugBuffer();
  bool addChar(char c);
  bool containsMatch();
  char* dataStart();

public:
  bool changed;
  PixelConfig pixelConfig;

  SerialReceiver(HardwareSerial *hardwareSerial);
  void loop();
  PixelConfig lastConfig();
};

#endif