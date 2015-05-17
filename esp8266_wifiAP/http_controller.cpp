#include "http_controller.h"

HttpController::HttpController(WifiWrapper *newWifiWrapper, PixelConfig *newPixelConfig) {
  wifiWrapper = newWifiWrapper;
  pixelConfig = newPixelConfig;
  PixelController controller(&Serial);
}

void HttpController::setup() {
  setupPages();
  httpServer.begin();
}

void HttpController::loop() {
  httpServer.handleClient();
}

String HttpController::wifiConfigForm() {
  String html = "<form action='/wifi_config' method='POST'>";
  html += "<h2>Wifi Config</h2>";
  html += "<div class='form-group'><label for='ssid'>SSID: </label><input class='form-control' name='ssid' maxlength='32' value='"; html += String(wifiConfig.ssid); html += "' /></div>";
  html += "<div class='form-group'><label for='password'>Password: </label><input class='form-control' name='password' value='"; html += String(wifiConfig.password); html += "' /></div>";
  html += "<div class='form-group'><label for='hostname'>Hostname: </label><input class='form-control' name='hostname' value='"; html += String(wifiConfig.hostname); html += "' /></div>";
  html += "<input class='btn btn-success' type='submit' /></form>";

  return html;
}

String HttpController::pixelConfigForm() {
  String html = "<form action='/pixel_config' method='POST'>";
  html += "<h2>Pixel Config</h2>";
  html += "<div class='form-group'><label for='red'>Number of Pixels: </label><input class='form-control' name='numPixels' maxlength='32' value='"; html += String((int)pixelConfig->numPixels); html += "' /></div>";
  html += "<div class='form-group'><label for='red'>Red: </label><input class='form-control' name='red' maxlength='32' value='"; html += String((int)pixelConfig->primaryColor.red); html += "' /></div>";
  html += "<div class='form-group'><label for='green'>Green: </label><input class='form-control' name='green' value='"; html += String((int)pixelConfig->primaryColor.green); html += "' /></div>";
  html += "<div class='form-group'><label for='blue'>Blue: </label><input class='form-control' name='blue' value='"; html += String((int)pixelConfig->primaryColor.blue); html += "' /></div>";
  html += "<input class='btn btn-success' type='submit' /></form>";

  return html;
}

void HttpController::setupPages() {
  httpServer.on("/wifi_config", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Wifi Configure");
    httpServer.send(200, "text/html", layout(wifiConfigForm()));
  });

  httpServer.on("/wifi_config", HTTP_POST, [this] () mutable {
    DEBUG_PRINTLN("POST Wifi Configure");
    String ssid = httpServer.arg("ssid");
    ssid.replace('+', ' ');
    
    ssid.toCharArray(wifiConfig.ssid, strlen(wifiConfig.ssid));
    httpServer.arg("password").toCharArray(wifiConfig.password, strlen(wifiConfig.password));
    httpServer.arg("hostname").toCharArray(wifiConfig.hostname, strlen(wifiConfig.hostname));

    wifiWrapper->setConfig(&wifiConfig);
    wifiWrapper->reconnect();
    WifiConfigRepository.persist(&wifiConfig);

    httpServer.send(200, "text/html", layout(layout(wifiConfigForm() + pixelConfigForm())));

    Serial.println("Restarting...");
    abort();
  });

  httpServer.on("/pixel_config", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Pixel Configure");
    httpServer.send(200, "text/html", layout(pixelConfigForm()));
  });

  httpServer.on("/pixel_config", HTTP_POST, [this] () mutable {
    DEBUG_PRINTLN("POST Pixel Configure");
    char buffer[4];

    httpServer.arg("numPixels").toCharArray(buffer, 4);
    pixelConfig->numPixels = (char)atoi(buffer);

    httpServer.arg("red").toCharArray(buffer, 4);
    pixelConfig->primaryColor.red = atoi(buffer);

    httpServer.arg("green").toCharArray(buffer, 4);
    pixelConfig->primaryColor.green = atoi(buffer);

    httpServer.arg("blue").toCharArray(buffer, 4);
    pixelConfig->primaryColor.blue = atoi(buffer);

    DEBUG_PRINT("Number of Pixels: ");
    DEBUG_PRINTLN2(pixelConfig->numPixels, DEC);
    DEBUG_PRINT("Red: ");
    DEBUG_PRINTLN2(pixelConfig->primaryColor.red, DEC);
    DEBUG_PRINT("Green: ");
    DEBUG_PRINTLN2(pixelConfig->primaryColor.green, DEC);
    DEBUG_PRINT("Blue: ");
    DEBUG_PRINTLN2(pixelConfig->primaryColor.blue, DEC);

    PixelConfigRepository.persist(pixelConfig);
    controller.send(pixelConfig);

    httpServer.send(200, "text/html", layout(layout(wifiConfigForm() + pixelConfigForm())));
  });

  httpServer.on("/resend", HTTP_GET, [this] () {
    controller.send(pixelConfig);
    httpServer.send(200, "text/html", layout(layout(wifiConfigForm() + pixelConfigForm())));
  });

  httpServer.on("/status", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Status");
    httpServer.send(200, "text/html", layout((String)wifiWrapper->getIP()));
  });

  httpServer.on("/", HTTP_GET, [this] () {
    DEBUG_PRINTLN("GET Root");
    httpServer.send(200, "text/html", layout(wifiConfigForm() + pixelConfigForm()));
  }); 
}

String HttpController::header() {
  String html = "<!doctype html5>\n";
  html += "<html>"; 
  html += "<head>";
  html += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css'>";
  html += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap-theme.min.css'>";
  html += "<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js'></script>";
  html += "</head>";
  html += "<body class='container'>";

  return html;
}

String HttpController::footer() {
  return "</body></html>";
}

String HttpController::layout(String contents) {
  return header() + contents + footer();
}

void HttpController::setWifiConfig(const WifiConfig *newConfig) {
  memcpy(&wifiConfig, newConfig, sizeof(wifiConfig));
}
