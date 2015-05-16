#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

typedef struct {
  char ssid[33];
  char password[65];
  char hostname[65];
} WifiConfig;

#endif