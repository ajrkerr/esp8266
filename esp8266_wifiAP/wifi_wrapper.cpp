#include "wifi_wrapper.h"

void WifiWrapper::setup(WifiConfig *newConfig) {
  setConfig(newConfig);
  connect();
}

bool WifiWrapper::connect() {
  if(!config.access_point && connectToAP()) {
    setupMDNS();
  } else {
    createAP();
  }

  DEBUG_PRINT("IP is: ");
  DEBUG_PRINTLN(getIP());

  return connected;
}

bool WifiWrapper::reconnect() {
  connect();
}

void WifiWrapper::loop() {
  mdnsServer.update();
}

IPAddress WifiWrapper::getIP() {
  if(config.access_point) {
    return WiFi.softAPIP();
  } else {
    return WiFi.localIP();
  }
}

void WifiWrapper::createAP() {
  DEBUG_PRINTLN("Going into AP Mode");
  debugConfig();

  if(strlen(config.password) == 0) {
    WiFi.softAP(config.ssid, config.password);
  } else {
    WiFi.softAP(config.ssid);
  }
  
  WiFi.mode(WIFI_AP);
}

void WifiWrapper::setupMDNS() {
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

bool WifiWrapper::connectToAP() {
  DEBUG_PRINTLN("Connecting using: ");
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

void WifiWrapper::debugConfig() {
  DEBUG_PRINT("SSID: ");
  DEBUG_PRINTLN(config.ssid);
  DEBUG_PRINT("Password: ");
  DEBUG_PRINTLN(config.password);
  DEBUG_PRINT("Hostname: ");
  DEBUG_PRINTLN(config.hostname);
}

char* WifiWrapper::getSSID() {
  return config.ssid;
}

char* WifiWrapper::getPassword() {
  return config.password;
}

char* WifiWrapper::getHostname() {
  return config.hostname;
}

void WifiWrapper::setConfig(const WifiConfig *newConfig) {
  memcpy(&config, newConfig, sizeof(config));
}

bool WifiWrapper::isConnected() {
  return connected;
}

