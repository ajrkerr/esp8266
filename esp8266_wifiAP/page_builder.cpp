#include "page_builder.h"

String PageBuilder::build(WifiConfig *wifiConfig, PixelConfig *pixelConfig, WiFiClient client) {
  String html = "";

  client.print("<!doctype html5>\n");
  client.print("<html>"); 
  client.print("<head>");
  client.print("<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css'>");
  client.print("<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap-theme.min.css'>");
  client.print("<script src='https://code.jquery.com/jquery-2.1.4.min.js'></script>");
  client.print("<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js'></script>");
  client.print("</head>");
  client.print("<body class='container'>");

  client.print("<script>");
  client.print("  $('#myTab a').click(function (e) {");
  client.print("    e.preventDefault();");
  client.print("    $(this).tab('show');");
  client.print("  })");
  client.print("</script>");
  client.print("<br />");
  client.print("<div role='tabpanel'>");
  client.print("  <ul class='nav nav-tabs' role='tablist'>");
  client.print("    <li role='presentation' class='active'><a href='#pixel-config' role='tab' data-toggle='tab'>Pixel Config</a></li>");
  client.print("    <li role='presentation'><a href='#wifi-config' role='tab' data-toggle='tab'>Wifi Config</a></li>");
  client.print("  </ul>");
  client.print("  <div class='tab-content'>");

  client.print("    <div role='tabpanel' class='tab-pane active' id='pixel-config'>");
  client.print("      <form action='/pixel_config' method='POST'>");
  client.print("      <h2>Pixel Config</h2>");
  client.print("      <div class='form-group'><label for='frameLength'>Frame Length: </label><input class='form-control' name='frameLength' maxlength='32' value='"); client.print(String((int)pixelConfig->frameLength)); client.print("' /></div>");
  client.print("      <div class='form-group'><label for='numPixels'>Number of Pixels: </label><input class='form-control' name='numPixels' maxlength='32' value='"); client.print(String((int)pixelConfig->numPixels)); client.print("' /></div>");
  client.print("      <div class='form-group'>");
  client.print("        <label for='type'>Animation Type: </label>");
  client.print("        <select class='form-control' name='type'>");
  client.print("          <option value='"); client.print(PIXEL_SOLID); client.print("'"); client.print((pixelConfig->type == PIXEL_SOLID) ? "selected" : ""); client.print(">Solid</option>");
  client.print("          <option value='"); client.print(PIXEL_TRACER); client.print("'"); client.print((pixelConfig->type == PIXEL_TRACER) ? "selected" : ""); client.print(">Tracer</option>");
  client.print("          <option value='"); client.print(PIXEL_RAINBOW); client.print("'"); client.print((pixelConfig->type == PIXEL_RAINBOW) ? "selected" : ""); client.print(">Rainbow</option>");
  client.print("          <option value='"); client.print(PIXEL_FLAME); client.print("'"); client.print((pixelConfig->type == PIXEL_FLAME) ? "selected" : ""); client.print(">Flame</option>");
  client.print("        </select>");
  client.print("      </div>");
  client.print("      <div class='row'><div class='col-xs-6'>");
  client.print("       <h4>Primary Color</h4>");
  client.print("       <div class='form-group'><label for='primary-red'>Red: </label><input class='form-control' name='primary-red' value='"); client.print(String((int)pixelConfig->primaryColor.red)); client.print("' /></div>");
  client.print("       <div class='form-group'><label for='primary-green'>Green: </label><input class='form-control' name='primary-green' value='"); client.print(String((int)pixelConfig->primaryColor.green)); client.print("' /></div>");
  client.print("       <div class='form-group'><label for='primary-blue'>Blue: </label><input class='form-control' name='primary-blue' value='"); client.print(String((int)pixelConfig->primaryColor.blue)); client.print("' /></div>");
  client.print("      </div><div class='col-xs-6'>");
  client.print("       <h4>Secondary Color</h4>");
  client.print("       <div class='form-group'><label for='secondary-red'>Red: </label><input class='form-control' name='secondary-red' value='"); client.print(String((int)pixelConfig->secondaryColor.red)); client.print("' /></div>");
  client.print("       <div class='form-group'><label for='secondary-green'>Green: </label><input class='form-control' name='secondary-green' value='"); client.print(String((int)pixelConfig->secondaryColor.green)); client.print("' /></div>");
  client.print("       <div class='form-group'><label for='secondary-blue'>Blue: </label><input class='form-control' name='secondary-blue' value='"); client.print(String((int)pixelConfig->secondaryColor.blue)); client.print("' /></div>");
  client.print("      </div></div>");
  client.print("      <input class='btn btn-success' type='submit' />");
  client.print("      </form>");
  client.print("    </div>");

  client.print("    <div role='tabpanel' class='tab-pane' id='wifi-config'>");
  client.print("      <form action='/wifi_config' method='POST'>");
  client.print("      <h2>Wifi Config</h2>");
  client.print("      <div class='form-group'><label for='ssid'>SSID: </label><input class='form-control' name='ssid' maxlength='32' value='"); client.print(String(wifiConfig->ssid)); client.print("' /></div>");
  client.print("      <div class='form-group'><label for='password'>Password: </label><input class='form-control' name='password' value='"); client.print(String(wifiConfig->password)); client.print("' /></div>");
  client.print("      <div class='form-group'><label for='hostname'>Hostname: </label><input class='form-control' name='hostname' value='"); client.print(String(wifiConfig->hostname)); client.print("' /></div>");
  client.print("      <input class='btn btn-success' type='submit' /></form>");
  client.print("    </div>");

  client.print("  </div>");
  client.print("</div>");

  client.print("</body>");
  client.print("</html>");

  return html;
}
