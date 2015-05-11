#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#include "esp8266_wifiAP.h"

ESP8266WebServer httpServer(80);
MDNSResponder mdnsServer;
WifiConfig config;

void setupWifiAP(const char *ssid) {
  Serial.println("Going into AP Mode");
  Serial.print("Setting up SSID: ");
  Serial.println(ssid);

  WiFi.softAP(ssid);
  WiFi.mode(WIFI_AP);

  Serial.print("IP is: ");
  Serial.println(WiFi.softAPIP());
}

void setupSerial(int baud) {
  Serial.begin(baud);
  delay(10);
  Serial.println("Serial is configured");
}

void setupMDNS(const char* hostname, IPAddress ip) {
  Serial.print("Setting up mDNS for: ");
  Serial.print(hostname);
  Serial.print(".local at ");
  Serial.println(ip);

  if(mdnsServer.begin(hostname, ip)) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up mDNS responder!");
  }
}

String configForm(WifiConfig *config) {
  String html = "<!doctype html>\n<html>";
  html += "<form method='POST'>";
  html += "<label for='ssid'>SSID: </label><input name='ssid' maxlength='32' value='"; html += config->ssid; html += "' /><br />";
  html += "<label for='password'>Password: </label><input name='password' value='"; html += config->password; html += "' /><br />";
  html += "<label for='hostname'>Hostname: </label><input name='hostname' value='"; html += config->hostname; html += "' /><br />";
  html += "<input type='submit' /></form>";
  html += "</html>";

  return html;
}

void setupWifiConfig(ESP8266WebServer *httpServer, WifiConfig *config) {
  httpServer->on("/configure", HTTP_GET, [httpServer, config] () {
    httpServer->send(200, "text/html", configForm(config));
  });

  httpServer->on("/configure", HTTP_POST, [httpServer, config] () mutable {
    Serial.println("Post received");

    String ssid = httpServer->arg("ssid");
    ssid.replace('+', ' ');
    ssid.toCharArray(config->ssid, 33);

    String password = httpServer->arg("password");
    password.toCharArray(config->password, 65);

    String hostname = httpServer->arg("hostname");
    hostname.toCharArray(config->hostname, 65);

    Serial.print("SSID: ");
    Serial.print(ssid);
    Serial.print("::");
    Serial.println(config->ssid);

    Serial.print("Password: ");
    Serial.print(password);
    Serial.print("::");
    Serial.println(config->password);

    Serial.print("Hostname: ");
    Serial.print(hostname);
    Serial.print("::");
    Serial.println(config->hostname);

    persistConfig(config);

    httpServer->send(200, "text/html", configForm(config));

    Serial.println("Restarting...");
    abort();
  });

  httpServer->begin();
}

boolean connectWireless(char *ssid, char *password) {
  Serial.print("Connecting to SSID: '");
  Serial.print(ssid);
  Serial.println("'");

  Serial.print("Connecting using password: '");
  Serial.print(password);
  Serial.println("'");

  Serial.print("WiFi connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while(WiFi.status() != WL_CONNECTED && WiFi.status() != WL_CONNECT_FAILED && attempts++ < 20) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() == WL_CONNECTED) {
    delay(50);
    Serial.print("Wifi connected at: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Wifi connection failed");
  }

  return WiFi.status() == WL_CONNECTED;
}

void setup() {
  setupSerial(SERIAL_BAUD);
  readConfig(&config);

  if(!connectWireless(config.ssid, config.password)) {
    setupWifiAP(WIFI_AP_SSID);
  } else {
    setupMDNS(config.hostname, WiFi.localIP());
  }

  setupWifiConfig(&httpServer, &config);
}

void loop() {
  httpServer.handleClient();
  mdnsServer.update();
}

void persistConfig(WifiConfig *config) {
  Serial.println("Persisting config");
  Serial.print("Size of config: ");
  Serial.println(sizeof(*config), DEC);

  EEPROM.begin(512);

  persistStructure(0, sizeof(*config), (char*)config);

  EEPROM.commit();
  EEPROM.end();

  Serial.println("Persisted Config:");
  Serial.print("SSID: ");
  Serial.println(config->ssid);
  Serial.print("Password: ");
  Serial.println(config->password);
  Serial.print("Hostname: ");
  Serial.println(config->hostname);
}

void readConfig(WifiConfig *config) {
  Serial.println("Reading config");

  EEPROM.begin(512);
  readStructure(0, sizeof(*config), (char*)config);
  EEPROM.end();

  config->ssid[32] = 0x0;
  config->password[64] = 0x0;
  config->hostname[64] = 0x0;

  Serial.println("Read Config:");
  Serial.print("SSID: ");
  Serial.println(config->ssid);
  Serial.print("Password: ");
  Serial.println(config->password);
  Serial.print("Hostname: ");
  Serial.println(config->hostname);
}

void persistStructure(int start_address, int size, char *structure) {
  for(int i = 0; i < size; i++) {
    EEPROM.write(start_address + i, structure[i]);
  }
}

void readStructure(int start_address, int size, char *structure) {
  for(int i = 0; i < size; i++) {
    structure[i] = EEPROM.read(start_address + i);
  }
}
