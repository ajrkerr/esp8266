#include "buffer.h"

Buffer::Buffer(unsigned int aStructureSize, char *aPreamble, unsigned int aPremableSize) {
  // Setup preamble, copy foreign preamble 
  preambleSize = aPremableSize;
  preamble = (char*) malloc(preambleSize);
  memcpy(preamble, aPreamble, preambleSize);

  structureSize = aStructureSize;
  bufferSize = structureSize + preambleSize * 2 + checksumSize;

  // Receiving data from 16bit ESP8266 means that we're receiving an even number of bytes
  // Last byte is padded out as 0x00
  if(structureSize % 2 != 0) bufferSize++;
  DEBUG_PRINT("Setting buffersize to: ");
  buffer = (char*) malloc(bufferSize);
  dataStart = buffer + preambleSize;
  checksum = buffer + bufferSize - preambleSize - 1;

  memset(buffer, 0x00, bufferSize);
}

void Buffer::add(char c) {
  for(int i = 0; i < bufferSize-1; i++) {
    buffer[i] = buffer[i+1];
  }

  buffer[bufferSize-1] = c;
}

bool Buffer::preambleValid() {
  for(int i = 0; i < preambleSize; i++) {
    if(buffer[i] != preamble[i]) {
      return false;
    }

    if(buffer[bufferSize - i - 1] != preamble[i]) {
      return false;
    }
  }

  return true;
}

bool Buffer::checksumValid() {
  if(*checksum == Checksum.calculate(dataStart, structureSize)) {
    DEBUG_PRINTLN("Checksum is valid");
    return true;
  } else {
    DEBUG_PRINT("Checksum: ");
    DEBUG_PRINT2(*checksum, DEC);
    DEBUG_PRINT(" does not match ");
    DEBUG_PRINTLN2(Checksum.calculate(dataStart, structureSize), DEC);
    return false;
  }
}

bool Buffer::containsMatch() {
  return preambleValid() && checksumValid();
}

void Buffer::copyIntoStructure(void *targetStructure) {
  memcpy(targetStructure, dataStart, structureSize);
}

void Buffer::debug() {
  DEBUG_PRINT("Buffer(");
  DEBUG_PRINT(bufferSize);
  DEBUG_PRINT(") : ");
  for(int i = 0; i < bufferSize; i++) {
    DEBUG_PRINT2((unsigned char)buffer[i], DEC);
    if(i != bufferSize -1) { 
      DEBUG_PRINT(", ");
    }
  }
  DEBUG_PRINTLN("");
}