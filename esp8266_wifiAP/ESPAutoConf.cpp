#include "ESPAutoConf.h"

ESPAutoConf::ESPAutoConf() {
}

void ESPAutoConf::setup(WifiConfig *newConfig) {
  setConfig(&config);
  connect();
}

bool ESPAutoConf::connect() {
  if(config.access_point) {
    connectToAP();
    setupMDNS();
  } else {
    createAP();
  }

  DEBUG_PRINT("IP is: ");
  DEBUG_PRINTLN(WiFi.getIP());

  return connected;
}

bool ESPAutoConf::reconnect() {
  // disconnect
  connect();
}

void ESPAutoConf::loop() {
  mdnsServer.update();
}

IPAddress ESPAutoConf::getIP() {
  if(config.access_point) {
    return WiFi.softAPIP();
  } else {
    return WiFi.localIP();
  }
}

void ESPAutoConf::createAP() {
  DEBUG_PRINTLN("Going into AP Mode");
  DEBUG_PRINT("Setting up SSID: ");
  DEBUG_PRINTLN(ssid);

  if(strlen(config.password) == 0) {
    WiFi.softAP(config.ssid, config.password);
  } else {
    WiFi.softAP(config.ssid);
  }
  
  WiFi.mode(WIFI_AP);
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

bool ESPAutoConf::connectToAP() {
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

void ESPAutoConf::setConfig(const WifiConfig *newConfig) {
  memcpy(&config, newConfig, sizeof(newConfig));
}

bool ESPAutoConf::isConnected() {
  return connected;
}

