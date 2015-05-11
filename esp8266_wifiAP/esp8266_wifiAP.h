
#define WIFI_AP_SSID "ConfigureESP8266"
#define SERIAL_BAUD 115200

typedef struct wifi_config {
  char ssid[33], password[65], hostname[65];
} WifiConfig;

String configForm(WifiConfig *config);

void setupWifiAP(const char *ssid);
void setupSerial(int baud);
void setupMDNS(const char *hostname, IPAddress source);
void setupWifiConfig(ESP8266WebServer *httpServer, WifiConfig *config);
boolean connectWireless(const char *ssid, const char *password);
void setup();
void loop();
void persistConfig(WifiConfig *config);
void readConfig(WifiConfig *config);

void persistStructure(int start_address, int size, char *structure);
void readStructure(int start_address, int size, char *structure);
