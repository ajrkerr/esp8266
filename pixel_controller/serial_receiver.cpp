#include "serial_receiver.h"

SerialReceiver::SerialReceiver(HardwareSerial *hardwareSerial) {
  serial = hardwareSerial;

  bufferSize = strlen(preamble) + strlen(postamble) + sizeof(tmpPixelConfig) + 1;
  serialBuffer = (char*)malloc(bufferSize);
  memset(serialBuffer, 0x00, bufferSize);
}

bool SerialReceiver::addChar(char c) {
  for(int i = 0; i < bufferSize-1; i++) {
    serialBuffer[i] = serialBuffer[i+1];
  }

  serialBuffer[bufferSize-1] = c;
}

bool SerialReceiver::containsMatch() {
  for(int i = 0; i < strlen(preamble); i++) {
    if(serialBuffer[i] != preamble[i]) {
      DEBUG_PRINTLN2(serialBuffer[i], DEC);
      return false;
    }
  }

  for(int i = 0; i < strlen(postamble); i++) {
    if(serialBuffer[bufferSize - i -1] != preamble[i]) {
      return false;
    }
  }

  return true;
}

char* SerialReceiver::dataStart() {
  return (char*) (serialBuffer + strlen(preamble));
}

void SerialReceiver::debugBuffer() {
  DEBUG_PRINT("Buffer Size: ");
  DEBUG_PRINTLN(bufferSize);
  DEBUG_PRINT("Current Buffer: ");
  for(int i = 0; i < bufferSize; i++) {
    DEBUG_PRINT2(serialBuffer[i], DEC);
    if(i != bufferSize -1) { 
      DEBUG_PRINT(", ");
    }
  }
  DEBUG_PRINTLN("");
}

void SerialReceiver::loop() {
  changed = false;

  while(serial->available() > 0) {
    addChar(serial->read());
    debugBuffer();

    if(containsMatch()) {
      DEBUG_PRINTLN("Preamble/Postamble found");

      memcpy(&tmpPixelConfig, dataStart(), sizeof(tmpPixelConfig));
      char checksum = dataStart()[sizeof(tmpPixelConfig)];

      if(checksum == Checksum.calculate(&tmpPixelConfig, sizeof(tmpPixelConfig))) {
        DEBUG_PRINTLN("Checksum is valid");
        memcpy(&pixelConfig, &tmpPixelConfig, sizeof(pixelConfig));
        changed = true;
      } else {
        DEBUG_PRINT("Checksum: ");
        DEBUG_PRINT2(checksum, DEC);
        DEBUG_PRINT(" does not match ");
        DEBUG_PRINTLN2(Checksum.calculate(&tmpPixelConfig, sizeof(tmpPixelConfig)), DEC);
      }
    }
  }
}