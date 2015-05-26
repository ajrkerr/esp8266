#include "http_controller.h"

void HttpController::setup(WifiWrapper *newWifiWrapper, PixelController *newPixelController, WifiConfig *newWifiConfig, PixelConfig *newPixelConfig) {
  setWifiConfig(newWifiConfig);
  setPixelConfig(newPixelConfig);

  setupPages();
  httpServer.begin();
}

void HttpController::loop() {
  httpServer.handleClient();
}

void HttpController::setupPages() {
  httpServer.on("/wifi_config", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Wifi Configure");
    httpServer.send(200, "text/html", "");
    pageBuilder.html(httpServer.client());
  });

  httpServer.on("/wifi_config", HTTP_POST, [this] () mutable {
    DEBUG_PRINTLN("POST Wifi Configure");
    String ssid = httpServer.arg("ssid");
    ssid.replace('+', ' ');
    
    ssid.toCharArray(wifiConfig.ssid, sizeof(wifiConfig.ssid));
    httpServer.arg("password").toCharArray(wifiConfig.password, sizeof(wifiConfig.password));
    httpServer.arg("hostname").toCharArray(wifiConfig.hostname, sizeof(wifiConfig.hostname));
    wifiConfig.access_point = false;

    WifiConfigRepository.persist(&wifiConfig);
    wifiWrapper->setConfig(&wifiConfig);
    wifiWrapper->reconnect();

    httpServer.send(200, "text/html", "");
    pageBuilder.html(httpServer.client());

    Serial.println("Restarting...");
    abort();
  });

  httpServer.on("/pixel_config", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Pixel Configure");
    httpServer.send(200, "text/html", "");
    pageBuilder.html(httpServer.client());
  });

  httpServer.on("/pixel_config.json", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Pixel Config JSON");
    httpServer.send(200, "text/html", PixelConfigSerializer.toJSON(&pixelConfig));
  });

  httpServer.on("/wifi_config.json", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Wifi Config JSON");
    httpServer.send(200, "text/html", WifiConfigSerializer.toJSON(&wifiConfig));
  });

  httpServer.on("/pixel_config", HTTP_POST, [this] () mutable {
    DEBUG_PRINTLN("POST Pixel Configure");

    pixelConfig.frameLength = httpServer.arg("frameLength").toInt();
    pixelConfig.numPixels = httpServer.arg("numPixels").toInt();
    pixelConfig.type = (Animation)httpServer.arg("type").toInt();

    pixelConfig.primaryColor.red = httpServer.arg("primary-red").toInt();
    pixelConfig.primaryColor.green = httpServer.arg("primary-green").toInt();
    pixelConfig.primaryColor.blue = httpServer.arg("primary-blue").toInt();

    pixelConfig.secondaryColor.red = httpServer.arg("secondary-red").toInt();
    pixelConfig.secondaryColor.green = httpServer.arg("secondary-green").toInt();
    pixelConfig.secondaryColor.blue = httpServer.arg("secondary-blue").toInt();

    DEBUG_PRINTLN("Persisting Data");
    PixelConfigRepository.persist(&pixelConfig);
    DEBUG_PRINTLN("Sending Data on Serial");
    pixelController->send(&pixelConfig);
    DEBUG_PRINTLN("Sending Header");
    httpServer.send(200, "text/html", "");
    DEBUG_PRINTLN("Sending Data");
    pageBuilder.html(httpServer.client());
  });

httpServer.on("/pixel_config.json", HTTP_POST, [this] () mutable {
    DEBUG_PRINTLN("POST Pixel Configure");

    pixelConfig.frameLength = httpServer.arg("frameLength").toInt();
    pixelConfig.numPixels = httpServer.arg("numPixels").toInt();
    pixelConfig.type = (Animation)httpServer.arg("type").toInt();

    pixelConfig.primaryColor.red = httpServer.arg("primary-red").toInt();
    pixelConfig.primaryColor.green = httpServer.arg("primary-green").toInt();
    pixelConfig.primaryColor.blue = httpServer.arg("primary-blue").toInt();

    pixelConfig.secondaryColor.red = httpServer.arg("secondary-red").toInt();
    pixelConfig.secondaryColor.green = httpServer.arg("secondary-green").toInt();
    pixelConfig.secondaryColor.blue = httpServer.arg("secondary-blue").toInt();

    DEBUG_PRINTLN("Persisting Data");
    PixelConfigRepository.persist(&pixelConfig);
    DEBUG_PRINTLN("Sending Data on Serial");
    pixelController->send(&pixelConfig);

    httpServer.send(200, "text/html", PixelConfigSerializer.toJSON(&pixelConfig));
  });

  httpServer.on("/resend", HTTP_GET, [this] () {
    pixelController->send(&pixelConfig);
    httpServer.send(200, "text/html", "");
    pageBuilder.html(httpServer.client());
  });

  httpServer.on("/", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Root");
    httpServer.send(200, "text/html", "");
    pageBuilder.html(httpServer.client());
  }); 

  httpServer.on("/script.js", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Script");
    httpServer.send(200, "text/html", "");
    pageBuilder.javascript(httpServer.client());
  }); 
}

void HttpController::setWifiConfig(const WifiConfig *newConfig) {
  memcpy(&wifiConfig, newConfig, sizeof(wifiConfig));
}

void HttpController::setPixelConfig(const PixelConfig *newConfig) {
  memcpy(&pixelConfig, newConfig, sizeof(pixelConfig));
}
