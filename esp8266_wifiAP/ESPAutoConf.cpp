#include "ESPAutoConf.h"

ESPAutoConf::ESPAutoConf() {
}

void ESPAutoConf::setup() {
  setup((char*)DEFAULT_HOSTNAME);
}

void ESPAutoConf::setup(char *configurationHostname) {
  loadConfig();

  if(connectWireless()) {
    setupMDNS();
  } else {
    setupWifiAP(configurationHostname);
  }
}

void ESPAutoConf::loop() {
  mdnsServer.update();
}

IPAddress ESPAutoConf::getIP() {
  if(connected) {
    return WiFi.localIP();
  } else {
    return WiFi.softAPIP();
  }
}

void ESPAutoConf::setupWifiAP(const char *ssid) {
  DEBUG_PRINTLN("Going into AP Mode");
  DEBUG_PRINT("Setting up SSID: ");
  DEBUG_PRINTLN(ssid);

  WiFi.softAP(ssid);
  WiFi.mode(WIFI_AP);

  DEBUG_PRINT("IP is: ");
  DEBUG_PRINTLN(WiFi.softAPIP());
}

void ESPAutoConf::setupMDNS() {
  DEBUG_PRINT("Setting up mDNS for: ");
  DEBUG_PRINT(config.hostname);
  DEBUG_PRINT(".local at ");
  DEBUG_PRINTLN(getIP());

  bool mdnsConnected = mdnsServer.begin(config.hostname, WiFi.localIP());

  if(mdnsConnected) {
    DEBUG_PRINTLN("mDNS responder started");
  } else {
    DEBUG_PRINTLN("Error setting up mDNS responder!");
  }
}

bool ESPAutoConf::connectWireless() {
  DEBUG_PRINT("Connecting using: '");
  debugConfig();
    
  WiFi.mode(WIFI_STA);
  WiFi.begin(config.ssid, config.password);

  int attempts = 0;
  while(WiFi.status() != WL_CONNECTED && WiFi.status() != WL_CONNECT_FAILED && attempts++ < MAX_WIFI_ATTEMPTS) {
    delay(500);
    DEBUG_PRINT(".");
  }
  DEBUG_PRINTLN("");

  connected = (WiFi.status() == WL_CONNECTED);

  if(connected) {
    delay(50);
    DEBUG_PRINT("Wifi connected at: ");
    DEBUG_PRINTLN(WiFi.localIP());
  } else {
    DEBUG_PRINTLN("Wifi connection failed");
  }

  return connected;
}

void ESPAutoConf::persistConfig() {
  WifiConfigRepository.persist(&config);
  DEBUG_PRINTLN("Persisted Config:");
  debugConfig();
}

void ESPAutoConf::loadConfig() {
  WifiConfigRepository.load(&config);
  DEBUG_PRINTLN("Read Config:");
  debugConfig();
}

void ESPAutoConf::debugConfig() {
  DEBUG_PRINT("SSID: ");
  DEBUG_PRINTLN(config.ssid);
  DEBUG_PRINT("Password: ");
  DEBUG_PRINTLN(config.password);
  DEBUG_PRINT("Hostname: ");
  DEBUG_PRINTLN(config.hostname);
}

char* ESPAutoConf::getSSID() {
  return config.ssid;
}

char* ESPAutoConf::getPassword() {
  return config.password;
}

char* ESPAutoConf::getHostname() {
  return config.hostname;
}

void ESPAutoConf::setSSID(const char *ssid) {
  strncpy(config.ssid, ssid, sizeof(config.ssid));
}

void ESPAutoConf::setPassword(const char *password) {
  strncpy(config.password, password, sizeof(config.password));
}

void ESPAutoConf::setHostname(const char *hostname) {
  strncpy(config.hostname, hostname, sizeof(config.hostname));
}

void ESPAutoConf::setSSID(String ssid) {
  ssid.toCharArray(config.ssid, sizeof(config.ssid));
}

void ESPAutoConf::setPassword(String password) {
  password.toCharArray(config.password, sizeof(config.password));
}

void ESPAutoConf::setHostname(String hostname) {
  hostname.toCharArray(config.hostname, sizeof(config.hostname));
}

void ESPAutoConf::setConfig(const WifiConfig newConfig) {
  strncpy(config.ssid, newConfig.ssid, sizeof(config.ssid));
  strncpy(config.password, newConfig.password, sizeof(config.password));
  strncpy(config.hostname, newConfig.hostname, sizeof(config.hostname));
}



