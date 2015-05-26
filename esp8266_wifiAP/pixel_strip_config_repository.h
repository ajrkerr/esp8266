#ifndef PIXEL_STRIP_CONFIG_REPOSITORY_H
#define PIXEL_STRIP_CONFIG_REPOSITORY_H

#include "repository.h"
#include "pixel_strip_config.h"

#define PIXEL_STRIP_CONFIG_LOCATION 255

class PixelStripConfigRepositoryClass {
public:
  void persist(PixelStripConfig *config) {
    Repository.persist(PIXEL_STRIP_CONFIG_LOCATION, sizeof(PixelStripConfig), (char*)config);
  }

  bool load(PixelStripConfig *config) {
    if(Repository.load(PIXEL_STRIP_CONFIG_LOCATION, sizeof(PixelStripConfig), (char*)config))
      return true;

    memset(config, 0x00, sizeof(*config));
    return false;
  }
};

extern PixelStripConfigRepositoryClass PixelStripConfigRepository;

#endif
