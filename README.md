# esp8266

Just some toy projects.

ESP8266_wifiAP -> Name is misleading. This takes a wifi configuration and pixel strip configuration to control a long strand of LEDS.  It takes the config and sends it via serial to a secondary MCU (ATMega328 in my case) and this second control actually drives the LEDs.
pixel_master -> Firmware for the ATMega328p which is driving the LEDs. This receives configuration updates from an ESP8266