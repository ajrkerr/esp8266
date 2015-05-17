#ifndef PIXEL_CONFIG_REPOSITORY_H
#define PIXEL_CONFIG_REPOSITORY_H

#include "repository.h"
#include "pixel_config.h"

#define PIXEL_CONFIG_LOCATION 255

class PixelConfigRepositoryClass {
public:
  void persist(PixelConfig *config) {
    Repository.persist(PIXEL_CONFIG_LOCATION, sizeof(*config), (char*)config);
  }

  bool load(PixelConfig *config) {
    if(Repository.load(PIXEL_CONFIG_LOCATION, sizeof(*config), (char*)config))
      return true;

    memset(config, 0x00, sizeof(*config));
    return false;
  }
};

extern PixelConfigRepositoryClass PixelConfigRepository;

#endif
