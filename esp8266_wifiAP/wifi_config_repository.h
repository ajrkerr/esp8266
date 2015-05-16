#ifndef WIFI_CONFIG_REPOSITORY_H
#define WIFI_CONFIG_REPOSITORY_H

#include "repository.h"
#include "wifi_config.h"

#define WIFI_CONFIG_LOCATION 0

class WifiConfigRepositoryClass {
public:
  void persist(WifiConfig *config) {
    Repository.persist(WIFI_CONFIG_LOCATION, sizeof(*config), (char*)config);
  }

  void load(WifiConfig *config) {
    Repository.load(WIFI_CONFIG_LOCATION, sizeof(*config), (char*)config);

    // Ensure config ends with string terminators
    config->ssid[sizeof(config->ssid) -1] = 0x0;
    config->password[sizeof(config->password) -1] = 0x0;
    config->hostname[sizeof(config->hostname) -1] = 0x0;
  }
};

extern WifiConfigRepositoryClass WifiConfigRepository;

#endif
