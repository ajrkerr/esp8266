#ifndef CHECKSUM_H
#define CHECKSUM_H

class ChecksumClass {
public:
  char calculate(void* data, int size) {
    char *cData = (char*)data;
    char checksum = 0x00;

    for(int i = 0; i < size; i++) {
      checksum = checksum ^ cData[i];
    }

    return checksum;
  }
};

extern ChecksumClass Checksum;

#endif