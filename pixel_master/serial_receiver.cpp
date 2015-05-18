#include "serial_receiver.h"

SerialReceiver::SerialReceiver(HardwareSerial *hardwareSerial) {
  serial = hardwareSerial;
  buffer = new Buffer(sizeof(PixelConfig), preamble, strlen(preamble));
}

void SerialReceiver::loop() {
  changed = false;

  while(serial->available() > 0) {
    buffer->add(serial->read());
    buffer->debug();

    if(buffer->containsMatch()) {
      DEBUG_PRINTLN("Preamble/Postamble found");
      buffer->copyIntoStructure(&pixelConfig);
      changed = true;
    }
  }
}