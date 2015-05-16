#ifndef ESPAUTOCONF_H
#define ESPAUTOCONF_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include "wifi_config.h"
#include "wifi_config_repository.h"
#include "debug_utils.h"

#define MAX_WIFI_ATTEMPTS 20
#define DEFAULT_HOSTNAME "ConfigureESP8266"
#define EEPROM_CONFIG_LOCATION 0

class ESPAutoConf {
  MDNSResponder mdnsServer;
  WifiConfig config;
  bool connected = false;

public:
  ESPAutoConf();

  void setup(char *configurationHostname);
  void setup();
  void loop();

  // Persistance Functions //
  void persistConfig();
  void loadConfig();

  void debugConfig();

  char* getSSID();
  char* getPassword();
  char* getHostname();

  void setSSID(const char *ssid);
  void setPassword(const char *password);
  void setHostname(const char *hostname);

  void setSSID(String ssid);
  void setPassword(String password);
  void setHostname(String hostname);

  void setConfig(const WifiConfig newConfig);

  IPAddress getIP();

private:
  // Core structure //
  void setupWifiAP(const char *ssid);
  void setupMDNS();
  bool connectWireless();
};

#endif