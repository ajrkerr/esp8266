#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <EEPROM.h>
#include "checksum.h"

#define BLOCK_SIZE 512

class RepositoryClass {
public:
  void persist(int start_address, int size, void *structure);
  bool load(int start_address, int size, void *structure);
};

extern RepositoryClass Repository;

#endif
