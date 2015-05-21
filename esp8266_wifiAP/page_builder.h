#ifndef PAGE_BUILDER_H
#define PAGE_BUILDER_H

#include <Arduino.h>

#include "wifi_config.h"
#include "pixel_config.h"
#include "WiFiClient.h"


class PageBuilder {
  WiFiClient client;

public:
  void html(WiFiClient client);
  void javascript(WiFiClient client);

private:
  void write(WiFiClient client, const char buffer[], int length);
};

PROGMEM const char javascriptData[] = R"###(
  var $http = function(url){ 
    var core = {
      ajax : function(method, url, args) {
        return new Promise(function(resolve, reject) {
          var client = new XMLHttpRequest();
          var uri = '';
          if((args != undefined) && (method == 'POST' || method == 'PUT')) {
            for (key in args) {
              uri += encodeURIComponent(key) + '=' + encodeURIComponent(escape(args[key])) + '&';
            }
            client.open(method, url + '?' + uri, true);
          } else {
            client.open(method, url, true);
          }

          client.onreadystatechange = function() {
            if(this.readyState == 4) {
              if(this.status == 200) {
                resolve(JSON.parse(this.response));
              } else{
                reject({"error":this.statusText});
              }
            }
          };
          client.send(uri);
        });
      }
    };

    return {
      'get' : function(args) { return core.ajax('GET', url, args); },
      'post' : function(args) { return core.ajax('POST', url, args); },
    };
  };

  // document.querySelectorAll

  var callback = {
    populate_pixel_config : function(data){
      $("[name=frameLength]").val(data.frameLength);
      $("[name=numPixels]").val(data.numPixels);
      $("[name=type]").val(data.type);
      $("[name=primary-red]").val(data.primaryColor.red);
      $("[name=primary-green]").val(data.primaryColor.green);
      $("[name=primary-blue]").val(data.primaryColor.blue);
      $("[name=secondary-red]").val(data.secondaryColor.red);
      $("[name=secondary-green]").val(data.secondaryColor.green);
      $("[name=secondary-blue]").val(data.secondaryColor.blue);
    },
    populate_wifi_config : function(data){
      $("[name=ssid]").val(data.ssid);
      $("[name=password]").val(data.password);
      $("[name=hostname]").val(data.hostname);
    },
    error : function(data){
      alert("Error loading configuration data from ESP8266");
    }
  };

  // Executes the method call
  $(function () { 
    $http("/pixel_config.json").get().then(callback.populate_pixel_config, callback.error);
    $http("/wifi_config.json").get().then(callback.populate_wifi_config, callback.error);
  });
)###";


PROGMEM const char htmlData[] = R"###(
<!doctype html5>
<html>
  <head>
    <script src='https://code.jquery.com/jquery-2.1.4.min.js'></script>
    <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css'>
    <script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js'></script>
    <script src='/script.js'></script>
    <script>
      $(function () { 
        $('.nav-tabs a').click(function (e) {
          e.preventDefault();
          $(this).tab('show');
        })
      });
    </script>
  </head>
  
  <body class='container'>
    <br />
    <div role='tabpanel'>
      <ul class='nav nav-tabs' role='tablist'>
        <li role='presentation' class='active'><a href='#pixel-config' role='tab' data-toggle='tab'>Pixel Config</a></li>
        <li role='presentation'><a href='#wifi-config' role='tab' data-toggle='tab'>Wifi Config</a></li>
      </ul>
      <div class='tab-content'>
        <div role='tabpanel' class='tab-pane active' id='pixel-config'>
          <form action='/pixel_config' method='POST'>
          <h2>Pixel Config</h2>
          <div class='form-group'><label for='frameLength'>Frame Length: </label><input class='form-control' name='frameLength' maxlength='32' /></div>
          <div class='form-group'><label for='numPixels'>Number of Pixels: </label><input class='form-control' name='numPixels' maxlength='32' /></div>
          <div class='form-group'>
            <label for='type'>Animation Type: </label>
            <select class='form-control' name='type'>
              <option value='0'>Solid</option>
              <option value='1'>Tracer</option>
              <option value='2'>Rainbow</option>
              <option value='3'>Flame</option>
            </select>
          </div>
          <div class='row'><div class='col-xs-6'>
           <h4>Primary Color</h4>
           <div class='form-group'><label for='primary-red'>Red: </label><input class='form-control' name='primary-red' /></div>
           <div class='form-group'><label for='primary-green'>Green: </label><input class='form-control' name='primary-green' /></div>
           <div class='form-group'><label for='primary-blue'>Blue: </label><input class='form-control' name='primary-blue' /></div>
          </div><div class='col-xs-6'>
           <h4>Secondary Color</h4>
           <div class='form-group'><label for='secondary-red'>Red: </label><input class='form-control' name='secondary-red' /></div>
           <div class='form-group'><label for='secondary-green'>Green: </label><input class='form-control' name='secondary-green' /></div>
           <div class='form-group'><label for='secondary-blue'>Blue: </label><input class='form-control' name='secondary-blue' /></div>
          </div></div>
          <input class='btn btn-success' type='submit' />
          </form>
        </div>

        <div role='tabpanel' class='tab-pane' id='wifi-config'>
          <form action='/wifi_config' method='POST'>
          <h2>Wifi Config</h2>
          <div class='form-group'><label for='ssid'>SSID: </label><input class='form-control' name='ssid' maxlength='32' /></div>
          <div class='form-group'><label for='password'>Password: </label><input class='form-control' name='password' /></div>
          <div class='form-group'><label for='hostname'>Hostname: </label><input class='form-control' name='hostname' /></div>
          <input class='btn btn-success' type='submit' /></form>
        </div>

      </div>
    </div>

  </body>
</html>
)###";

#endif