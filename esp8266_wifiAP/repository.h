#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <EEPROM.h>

#define BLOCK_SIZE 512

class RepositoryClass {
public:
  void persist(int start_address, int size, char *structure);
  void load(int start_address, int size, char *structure);
};

extern RepositoryClass Repository;

#endif
