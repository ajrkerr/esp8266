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
  html += "<div class='form-group'><label for='frameLength'>Frame Length: </label><input class='form-control' name='frameLength' maxlength='32' value='"; html += String((int)pixelConfig->frameLength); html += "' /></div>";
  html += "<div class='form-group'><label for='numPixels'>Number of Pixels: </label><input class='form-control' name='numPixels' maxlength='32' value='"; html += String((int)pixelConfig->numPixels); html += "' /></div>";
  html += "<div class='form-group'><label for='type'>Animation Type: </label>";
  html += "  <select class='form-control' name='type'>";
  html += "    <option value='"; html += PIXEL_SOLID; html += "'"; html += (pixelConfig->type == PIXEL_SOLID) ? "selected" : ""; html += ">Solid</option>";
  html += "    <option value='"; html += PIXEL_TRACER; html += "'"; html += (pixelConfig->type == PIXEL_TRACER) ? "selected" : ""; html += ">Tracer</option>";
  html += "    <option value='"; html += PIXEL_RAINBOW; html += "'"; html += (pixelConfig->type == PIXEL_RAINBOW) ? "selected" : ""; html += ">Rainbow</option>";
  html += "    <option value='"; html += PIXEL_FLAME; html += "'"; html += (pixelConfig->type == PIXEL_FLAME) ? "selected" : ""; html += ">Flame</option>";
  html += "  </select>";
  html += "</div>";
  html += "<h4>Primary Color</h4>";
  html += "<div class='form-group'><label for='primary-red'>Red: </label><input class='form-control' name='primary-red' value='"; html += String((int)pixelConfig->primaryColor.red); html += "' /></div>";
  html += "<div class='form-group'><label for='primary-green'>Green: </label><input class='form-control' name='primary-green' value='"; html += String((int)pixelConfig->primaryColor.green); html += "' /></div>";
  html += "<div class='form-group'><label for='primary-blue'>Blue: </label><input class='form-control' name='primary-blue' value='"; html += String((int)pixelConfig->primaryColor.blue); html += "' /></div>";
  html += "<h4>Secondary Color</h4>";
  html += "<div class='form-group'><label for='secondary-red'>Red: </label><input class='form-control' name='secondary-red' value='"; html += String((int)pixelConfig->secondaryColor.red); html += "' /></div>";
  html += "<div class='form-group'><label for='secondary-green'>Green: </label><input class='form-control' name='secondary-green' value='"; html += String((int)pixelConfig->secondaryColor.green); html += "' /></div>";
  html += "<div class='form-group'><label for='secondary-blue'>Blue: </label><input class='form-control' name='secondary-blue' value='"; html += String((int)pixelConfig->secondaryColor.blue); html += "' /></div>";
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

    pixelConfig->frameLength = httpServer.arg("frameLength").toInt();
    pixelConfig->numPixels = httpServer.arg("numPixels").toInt();
    pixelConfig->type = (Animation)httpServer.arg("type").toInt();

    pixelConfig->primaryColor.red = httpServer.arg("primary-red").toInt();
    pixelConfig->primaryColor.green = httpServer.arg("primary-green").toInt();
    pixelConfig->primaryColor.blue = httpServer.arg("primary-blue").toInt();

    pixelConfig->secondaryColor.red = httpServer.arg("secondary-red").toInt();
    pixelConfig->secondaryColor.green = httpServer.arg("secondary-green").toInt();
    pixelConfig->secondaryColor.blue = httpServer.arg("secondary-blue").toInt();

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
