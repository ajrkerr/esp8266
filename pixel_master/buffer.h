#ifndef BUFFER_H
#define BUFFER_H

#include <Arduino.h>
#include "checksum.h"
#include "debug_utils.h"

class Buffer {
  unsigned int structureSize;
  unsigned int preambleSize;
  unsigned int bufferSize;
  unsigned int checksumSize = 1;

  char *buffer;
  char *preamble;
  char *checksum;
  char *dataStart;

public:

  Buffer(unsigned int newStructureSize, char *newPreamble, unsigned int newPremableSize);
  void add(char c);
  bool preambleValid();
  bool checksumValid();
  bool containsMatch();
  void copyIntoStructure(void *targetStructure);
  void debug();
};

#endif