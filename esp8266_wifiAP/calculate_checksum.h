#ifndef CALCULATE_CHECKSUM_H
#define CALCULATE_CHECKSUM_H

#define CIRCULAR_SHIFT(x) ((x >> 1) | ((x & 0x01) << 7))

char calculate_checksum(void* data, int size) {
  char *cData = (char*)data;
  char checksum = 0x00;

  for(int i = 0; i < size; i++) {
    checksum = CIRCULAR_SHIFT(checksum) ^ cData[i];
  }

  return checksum;
}

#endif