#ifndef WIFI_WRAPPER_H
#define WIFI_WRAPPER_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include "wifi_config.h"
#include "debug_utils.h"

#define MAX_WIFI_ATTEMPTS 20

class WifiWrapper {
  MDNSResponder mdnsServer;
  WifiConfig config;
  bool connected = false;

public:
  void setup(WifiConfig *newConfig);
  void loop();
  bool connect();
  bool reconnect();

  void debugConfig();

  char* getSSID();
  char* getPassword();
  char* getHostname();

  void setConfig(const WifiConfig *newConfig);
  void createAP();
  bool connectToAP();
  bool isConnected();

  IPAddress getIP();

private:
  // Core structure //
  void setupWifiAP(const char *ssid);
  void setupMDNS();
  bool connectWireless();
};

#endif