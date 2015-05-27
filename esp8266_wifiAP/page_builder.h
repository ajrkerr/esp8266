#ifndef PAGE_BUILDER_H
#define PAGE_BUILDER_H

#include <Arduino.h>

#include "wifi_config.h"
#include "pixel_strip_config.h"
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

          client.open(method, url, true);

          client.onreadystatechange = function() {
            if(this.readyState == 4) {
              if(this.status == 200) {
                resolve(JSON.parse(this.response));
              } else{
                reject({'error':this.statusText});
              }
            }
          };

          if((args != undefined) && (method == 'POST' || method == 'PUT')) {
            for (key in args) {
              uri += encodeURIComponent(key) + '=' + encodeURIComponent(escape(args[key])) + '&';
            }

            client.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
          }

          client.send(uri);
        });
      }
    };

    return {
      'get' : function(args) { return core.ajax('GET', url, args); },
      'post' : function(args) { return core.ajax('POST', url, args); },
    };
  };

  function select(context, selector) {
    if(selector === undefined || selector === null) {
      selector = context;
      context = document;
    }

    return context.querySelectorAll(selector);
  }

  // Color //
  function Color(color) {
    this.red = color.red;
    this.green = color.green;
    this.blue = color.blue;
  }
  Color.fromHex = function (hex) {
    if(hex.charAt(0) !== '#' && hex.length === 7) {
      throw 'Invalid hex color.  Should be formatted #RRGGBB.  Got "' + hex + '" instead.'
    }

    function parseHex(hex) { return parseInt(hex, 16); }

    return new Color({
      'red': parseHex(hex.substring(1, 3)),
      'blue': parseHex(hex.substring(3, 5)),
      'green': parseHex(hex.substring(5, 7)),
    });
  }
  Color.prototype.toHtmlColor = function () {
    var toHex = function (int) {
      return ((int < 16) ?  '0' + int.toString(16) : int.toString(16));
    }
    return '#' + toHex(this.red) + toHex(this.green) + toHex(this.blue);
  }


  // PixelStripConfig //
  function PixelStripConfig() {}
  PixelStripConfig.fromJSON = function (json) {
    var config = new PixelStripConfig();

    config.frameLength = json.frameLength;
    config.numPixels = json.numPixels;
    config.type = json.type;

    config.primaryColor = new Color(json.primaryColor);
    config.secondaryColor = new Color(json.secondaryColor);

    return config;
  }
  PixelStripConfig.fromFormData = function (formData) {
    var config = new PixelStripConfig();

    config.frameLength = formData.frameLength;
    config.numPixels = formData.numPixels;
    config.type = formData.type;

    config.primaryColor = Color.fromHex(formData.primaryColor);
    config.secondaryColor = Color.fromHex(formData.secondaryColor);

    return config;
  }

  // Pixel Config Serializer
  function PixelStripConfigSerializer() {}
  PixelStripConfigSerializer.serialize = function(config) {
    return {
      'frameLength': config.frameLength,
      'numPixels': config.numPixels,
      'type': config.type,

      'primary-red': config.primaryColor.red,
      'primary-blue': config.primaryColor.blue,
      'primary-green': config.primaryColor.green,

      'secondary-red': config.secondaryColor.red,
      'secondary-blue': config.secondaryColor.blue,
      'secondary-green': config.secondaryColor.green,
    }
  }

  // PixelForm //
  function PixelForm(form, config) {
    this.config = config;
    this.form = form;

    this.bindToUI();
    this.populateForm();
  }
  PixelForm.prototype.bindToUI = function () {
    this.ui = {
      frameLength: select(this.form, '[name=frameLength]')[0],
      numPixels: select(this.form, '[name=numPixels]')[0],
      type: select(this.form, '[name=type]')[0],
      primaryColor: select(this.form, '[name=primary-color]')[0],
      secondaryColor: select(this.form, '[name=secondary-color]')[0],
    }

    var that = this;
    this.form.addEventListener('submit', function (e) { that.submit(e); });
  }
  PixelForm.prototype.populateForm = function () {
    this.ui.frameLength.value = this.config.frameLength;
    this.ui.numPixels.value = this.config.numPixels;
    this.ui.type.selectedIndex = this.config.type;

    if(typeof $ !== 'undefined' && typeof $.minicolors !== 'undefined') {
      $(this.ui.primaryColor).minicolors('value', this.config.primaryColor.toHtmlColor());
      $(this.ui.secondaryColor).minicolors('value', this.config.secondaryColor.toHtmlColor());
    } else {
      this.ui.primaryColor.value = this.config.primaryColor.toHtmlColor(); 
      this.ui.secondaryColor.value = this.config.secondaryColor.toHtmlColor();
    }
  }
  PixelForm.prototype.getFormData = function () {
    var form = this;
    return Object.keys(this.ui).reduce(function (accumulator, key) {
      accumulator[key] = form.ui[key].value;
      return accumulator;
    }, {})
  }

  PixelForm.prototype.submit = function (e) {
    e.preventDefault();
    
    var formData = this.getFormData();
    console.log(formData);
    var config = PixelStripConfig.fromFormData(formData);
    var serializedConfig = PixelStripConfigSerializer.serialize(config);

    function addBannerFromTemplate(template) {
      var alertTemplate = document.getElementById(template);
      var newAlert = alertTemplate.cloneNode(true);
      newAlert.classList.remove('template');

      var container = document.getElementbyId('container');

      container.insertBefore(newAlert, container.firstChild);
    }

    function showSuccess() {
      addBannerFromTemplate('success-template');
    }

    function showFailure() {
      addBannerFromTemplate('success-template');
    }

    $http('/pixel_strip_config.json')
      .post(serializedConfig)
      .then(showSuccess, showFailure);
  }

  var callback = {
    buildPixelForm : function(data){
      var config = PixelStripConfig.fromJSON(data);
      var form = document.getElementById('pixel-config-form');
      new PixelForm(form, config);
    },

    buildWifiForm : function(data){
      select('[name=ssid]')[0].value = data.ssid;
      select('[name=password]')[0].value = data.password;
      select('[name=hostname]')[0].value = data.hostname;
    },
    error : function(data){
      alert('Error loading configuration data from ESP8266');
    }
  };


  function onLoad() {
    $http('/pixel_strip_config.json').get().then(callback.buildPixelForm, callback.error);
    $http('/wifi_config.json').get().then(callback.buildWifiForm, callback.error);
  }

  
  if (document.readyState != 'loading'){
    onLoad();
  } else {
    document.addEventListener('DOMContentLoaded', onLoad);
  }


)###";


PROGMEM const char htmlData[] = R"###(
<!doctype html5>
<html>
  <head>
    <script src='https://code.jquery.com/jquery-2.1.4.min.js'></script>
    <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css'>
    <script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/js/bootstrap.min.js'></script>

    <link rel='stylesheet' href='https://rawgit.com/claviska/jquery-minicolors/master/jquery.minicolors.css'>
    <script src='https://rawgit.com/claviska/jquery-minicolors/master/jquery.minicolors.min.js'></script>

    <script src='/script.js'></script>
    <script>
      $(function () { 
        $('.nav-tabs a').click(function (e) {
          e.preventDefault();
          $(this).tab('show');
        });
        $('.color-picker').minicolors({theme: 'bootstrap'});
      });
    </script>
    <style>
      .template {
        display: none;
      }
    </style>
  </head>
  
  <body class='container' id='container'>
    <div id='success-template' class='template alert alert-success alert-dismissible' role='alert'>
      <button type='button' class='close' data-dismiss='alert'><span>&times;</span></button>
      <strong>Success!</strong> The configuration has been updated.
    </div>

    <br />
    <div role='tabpanel'>
      <ul class='nav nav-tabs' role='tablist'>
        <li role='presentation' class='active'><a href='#pixel-config' role='tab' data-toggle='tab'>Pixel Config</a></li>
        <li role='presentation'><a href='#wifi-config' role='tab' data-toggle='tab'>Wifi Config</a></li>
      </ul>
      <div class='tab-content'>
        <div role='tabpanel' class='tab-pane active' id='pixel-config'>
          <form action='/pixel_strip_config' method='POST' id='pixel-config-form'>
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
            <div class='row'>
              <div class='col-xs-6'>
                <div class='form-group'>
                  <label for='primary-red'>Primary Color: </label>
                  <input class='form-control color-picker' name='primary-color' />
                </div>
              </div>
              <div class='col-xs-6'>
                <div class='form-group'>
                  <label for='secondary-red'>Secondary Color: </label>
                  <input class='form-control color-picker' name='secondary-color' />
                </div>
              </div>
            </div>
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