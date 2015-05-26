#include "serial_receiver.h"

SerialReceiver::SerialReceiver(HardwareSerial *hardwareSerial) {
  serial = hardwareSerial;
  DEBUG_PRINTLN("Creating buffer");
  DEBUG_PRINTLN(String(strlen(preamble)));
  DEBUG_PRINTLN(String(sizeof(PixelStripConfig)));
  buffer = new Buffer(sizeof(PixelStripConfig), preamble, strlen(preamble));
}

void SerialReceiver::loop() {
  changed = false;

  while(serial->available() > 0) {
    buffer->add(serial->read());
    buffer->debug();

    if(buffer->containsMatch()) {
      DEBUG_PRINTLN("Preamble/Postamble found");
      buffer->copyIntoStructure(&pixelStripConfig);
      changed = true;
    }
  }
}