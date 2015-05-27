#include "http_controller.h"

void HttpController::setup(WifiWrapper *aWifiWrapper, PixelStripConfigSender *aPixelStripConfigSender) {
  wifiWrapper = aWifiWrapper;
  pixelStripConfigSender = aPixelStripConfigSender;
  
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
    wifiWrapper->setConfig(wifiConfig);
    wifiWrapper->reconnect();

    httpServer.send(200, "text/html", "");
    pageBuilder.html(httpServer.client());

    Serial.println("Restarting...");
    abort();
  });

  httpServer.on("/pixel_strip_config", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Pixel Configure");
    httpServer.send(200, "text/html", "");
    pageBuilder.html(httpServer.client());
  });

  httpServer.on("/pixel_strip_config.json", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Pixel Config JSON");
    httpServer.send(200, "text/html", PixelStripConfigSerializer.toJSON(&pixelStripConfig));
  });

  httpServer.on("/wifi_config.json", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Wifi Config JSON");
    httpServer.send(200, "text/html", WifiConfigSerializer.toJSON(&wifiConfig));
  });

  httpServer.on("/pixel_strip_config", HTTP_POST, [this] () mutable {
    DEBUG_PRINTLN("POST Pixel Configure");

    pixelStripConfig.frameLength = httpServer.arg("frameLength").toInt();
    pixelStripConfig.numPixels = httpServer.arg("numPixels").toInt();
    pixelStripConfig.type = (AnimationType)httpServer.arg("type").toInt();

    pixelStripConfig.primaryColor.red = httpServer.arg("primary-red").toInt();
    pixelStripConfig.primaryColor.green = httpServer.arg("primary-green").toInt();
    pixelStripConfig.primaryColor.blue = httpServer.arg("primary-blue").toInt();

    pixelStripConfig.secondaryColor.red = httpServer.arg("secondary-red").toInt();
    pixelStripConfig.secondaryColor.green = httpServer.arg("secondary-green").toInt();
    pixelStripConfig.secondaryColor.blue = httpServer.arg("secondary-blue").toInt();

    DEBUG_PRINTLN("Persisting Data");
    PixelStripConfigRepository.persist(&pixelStripConfig);
    DEBUG_PRINTLN("Sending Data on Serial");
    pixelStripConfigSender->send(pixelStripConfig);
    DEBUG_PRINTLN("Sending Header");
    httpServer.send(200, "text/html", "");
    DEBUG_PRINTLN("Sending Data");
    pageBuilder.html(httpServer.client());
  });

  httpServer.on("/pixel_strip_config.json", HTTP_POST, [this] () mutable {
    DEBUG_PRINTLN("POST Pixel Configure");

    pixelStripConfig.frameLength = httpServer.arg("frameLength").toInt();
    pixelStripConfig.numPixels = httpServer.arg("numPixels").toInt();
    pixelStripConfig.type = (AnimationType)httpServer.arg("type").toInt();

    pixelStripConfig.primaryColor.red = httpServer.arg("primary-red").toInt();
    pixelStripConfig.primaryColor.green = httpServer.arg("primary-green").toInt();
    pixelStripConfig.primaryColor.blue = httpServer.arg("primary-blue").toInt();

    pixelStripConfig.secondaryColor.red = httpServer.arg("secondary-red").toInt();
    pixelStripConfig.secondaryColor.green = httpServer.arg("secondary-green").toInt();
    pixelStripConfig.secondaryColor.blue = httpServer.arg("secondary-blue").toInt();

    DEBUG_PRINTLN("Persisting Data");
    PixelStripConfigRepository.persist(&pixelStripConfig);
    DEBUG_PRINTLN("Sending Data on Serial");
    pixelStripConfigSender->send(pixelStripConfig);

    httpServer.send(200, "text/html", PixelStripConfigSerializer.toJSON(&pixelStripConfig));
  });

  httpServer.on("/resend", HTTP_GET, [this] () {
    pixelStripConfigSender->send(pixelStripConfig);
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
