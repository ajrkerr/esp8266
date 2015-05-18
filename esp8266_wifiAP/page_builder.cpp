#include "page_builder.h"

String PageBuilder::build(WifiConfig *wifiConfig, PixelConfig *pixelConfig) {
  String html = "";

  html += "<!doctype html5>\n";
  html += "<html>"; 
  html += "<head>";
  html += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css'>";
  html += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap-theme.min.css'>";
  html += "<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js'></script>";
  html += "</head>";
  html += "<body class='container'>";

  html += "<script>";
  html += "  $('#myTab a').click(function (e) {";
  html += "    e.preventDefault()";
  html += "    $(this).tab('show')";
  html += "  })";
  html += "</script>";
  html += "<div role='tabpanel'>";
  html += "  <ul class='nav nav-tabs' role='tablist'>";
  html += "    <li role='presentation' class='active'><a href='#pixel-config' role='tab' data-toggle='tab'>Pixel Config</a></li>";
  html += "    <li role='presentation'><a href='#wifi-config' role='tab' data-toggle='tab'>Wifi Config</a></li>";
  html += "  </ul>";
  html += "  <div class='tab-content'>";

  html += "    <div role='tabpanel' class='tab-pane active' id='pixel-config'>";
  html += "      <form action='/pixel_config' method='POST'>";
  html += "      <h2>Pixel Config</h2>";
  html += "      <div class='form-group'><label for='frameLength'>Frame Length: </label><input class='form-control' name='frameLength' maxlength='32' value='"; html += String((int)pixelConfig->frameLength); html += "' /></div>";
  html += "      <div class='form-group'><label for='numPixels'>Number of Pixels: </label><input class='form-control' name='numPixels' maxlength='32' value='"; html += String((int)pixelConfig->numPixels); html += "' /></div>";
  html += "      <div class='form-group'>";
  html += "        <label for='type'>Animation Type: </label>";
  html += "        <select class='form-control' name='type'>";
  html += "          <option value='"; html += PIXEL_SOLID; html += "'"; html += (pixelConfig->type == PIXEL_SOLID) ? "selected" : ""; html += ">Solid</option>";
  html += "          <option value='"; html += PIXEL_TRACER; html += "'"; html += (pixelConfig->type == PIXEL_TRACER) ? "selected" : ""; html += ">Tracer</option>";
  html += "          <option value='"; html += PIXEL_RAINBOW; html += "'"; html += (pixelConfig->type == PIXEL_RAINBOW) ? "selected" : ""; html += ">Rainbow</option>";
  html += "          <option value='"; html += PIXEL_FLAME; html += "'"; html += (pixelConfig->type == PIXEL_FLAME) ? "selected" : ""; html += ">Flame</option>";
  html += "        </select>";
  html += "      </div>";
  html += "      <div class='row'><div class='col-md-6'>";
  html += "       <h4>Primary Color</h4>";
  html += "       <div class='form-group'><label for='primary-red'>Red: </label><input class='form-control' name='primary-red' value='"; html += String((int)pixelConfig->primaryColor.red); html += "' /></div>";
  html += "       <div class='form-group'><label for='primary-green'>Green: </label><input class='form-control' name='primary-green' value='"; html += String((int)pixelConfig->primaryColor.green); html += "' /></div>";
  html += "       <div class='form-group'><label for='primary-blue'>Blue: </label><input class='form-control' name='primary-blue' value='"; html += String((int)pixelConfig->primaryColor.blue); html += "' /></div>";
  html += "      </div><div class='col-md-6'>";
  html += "       <h4>Secondary Color</h4>";
  html += "       <div class='form-group'><label for='secondary-red'>Red: </label><input class='form-control' name='secondary-red' value='"; html += String((int)pixelConfig->secondaryColor.red); html += "' /></div>";
  html += "       <div class='form-group'><label for='secondary-green'>Green: </label><input class='form-control' name='secondary-green' value='"; html += String((int)pixelConfig->secondaryColor.green); html += "' /></div>";
  html += "       <div class='form-group'><label for='secondary-blue'>Blue: </label><input class='form-control' name='secondary-blue' value='"; html += String((int)pixelConfig->secondaryColor.blue); html += "' /></div>";
  html += "      </div></div>";
  html += "      <input class='btn btn-success' type='submit' />";
  html += "      </form>";
  html += "    </div>";

  html += "    <div role='tabpanel' class='tab-pane' id='wifi-config'>";
  html += "      <form action='/wifi_config' method='POST'>";
  html += "      <h2>Wifi Config</h2>";
  html += "      <div class='form-group'><label for='ssid'>SSID: </label><input class='form-control' name='ssid' maxlength='32' value='"; html += String(wifiConfig->ssid); html += "' /></div>";
  html += "      <div class='form-group'><label for='password'>Password: </label><input class='form-control' name='password' value='"; html += String(wifiConfig->password); html += "' /></div>";
  html += "      <div class='form-group'><label for='hostname'>Hostname: </label><input class='form-control' name='hostname' value='"; html += String(wifiConfig->hostname); html += "' /></div>";
  html += "      <input class='btn btn-success' type='submit' /></form>";
  html += "    </div>";

  html += "  </div>";
  html += "</div>";

  html += "</body>";
  html += "</html>";

  return html;
}
