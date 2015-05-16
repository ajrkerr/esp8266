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

  void load(PixelConfig *config) {
   Repository.load(PIXEL_CONFIG_LOCATION, sizeof(*config), (char*)config);
  }
};

extern PixelConfigRepositoryClass PixelConfigRepository;

#endif
