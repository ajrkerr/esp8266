#include "Repository.h"

void RepositoryClass::persist(int start_address, int size, char *structure) {
  EEPROM.begin(BLOCK_SIZE);

  for(int i = 0; i < size; i++) {
    EEPROM.write(start_address + i, structure[i]);
  }

  EEPROM.commit();
  EEPROM.end();
}

void RepositoryClass::load(int start_address, int size, char *structure) {
  EEPROM.begin(BLOCK_SIZE);

  for(int i = 0; i < size; i++) {
    structure[i] = EEPROM.read(start_address + i);
  }

  EEPROM.end();
}

RepositoryClass Repository;