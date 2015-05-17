#include "Repository.h"

void RepositoryClass::persist(int start_address, int size, void *structure) {
  char *cStructure = (char*)structure;

  EEPROM.begin(BLOCK_SIZE);

  for(int i = 0; i < size; i++) {
    EEPROM.write(start_address + i, cStructure[i]);
  }

  EEPROM.write(start_address + size, Checksum.calculate(structure, size));

  EEPROM.commit();
  EEPROM.end();
}

bool RepositoryClass::load(int start_address, int size, void *structure) {
  char *cStructure = (char*)structure;
  EEPROM.begin(BLOCK_SIZE);

  for(int i = 0; i < size; i++) {
    cStructure[i] = EEPROM.read(start_address + i);
  }
  
  char checksum = EEPROM.read(start_address + size);

  EEPROM.end();

  return checksum == Checksum.calculate(structure, size);
}

RepositoryClass Repository;