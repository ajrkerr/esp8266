#ifndef PIXEL_STRIP_H
#define PIXEL_STRIP_H

#include <Arduino.h>
#include "FastLED.h"
#include "pixel.h"
#include "pixel_strip_config.h"

class PixelStrip {
protected:
  CRGB *leds;
  int dataPin;
  int numPixels;

public:
  virtual void setPixel(int position, Pixel pixel);
  virtual Pixel getPixel(int position);
  virtual void draw();
  virtual void resize(int aNumPixels);
};

class WSPixelStrip : public PixelStrip {
public:
  WSPixelStrip(int aDataPin, int aNumPixels) {
    dataPin = aDataPin;
    numPixels = aNumPixels;

    leds = (CRGB*) malloc(sizeof(CRGB) * numPixels);
    FastLED.addLeds<WS2812B, PIXEL_DRIVER_PIN, RGB>(leds, numPixels);
  }

  void setPixel(int position, Pixel pixel) {
    leds[position].red = pixel.red;
    leds[position].blue = pixel.blue;
    leds[position].green = pixel.green;
  }

  Pixel getPixel(int position) { Pixel pixel; return pixel; }
  void draw() { FastLED.show(); }
  
  void resize(int aNumPixels) {
    numPixels = aNumPixels;
    FastLED.addLeds<WS2812B, PIXEL_DRIVER_PIN, RGB>(leds, numPixels);
    FastLED.clear();
  }
};

// #include <Adafruit_NeoPixel.h>

// class NeoPixelStrip : PixelStrip{
//   Adafruit_NeoPixel *strip;

// public:
//   NeoPixelStrip(int aDataPin, int numPixels) {
//     dataPin = aDataPin;
//     numPixels = aNumPixels;
    
//     strip = new Adafruit_NeoPixel(numPixels, dataPin, NEO_GRB + NEO_KHZ800);
//     strip->begin();
//   }

//   void setPixel(int position, Pixel pixel) {
//     strip->setPixelColor(position, pixel.red, pixel.green, pixel.blue);
//   }

//   Pixel getPixel(int position) {
//     uint32_t color = strip->getPixelColor(position);
//     Pixel pixel;
//     pixel.green = 0xFF0000 & color;
//     pixel.red   = 0x00FF00 & color;
//     pixel.blue  = 0x0000FF & color;
//     return pixel;
//   }

//   void draw() { strip->show(); }
//   void resize(int aNumPixels) {
//     free(strip);
//     strip = new Adafruit_NeoPixel(numPixels, dataPin, NEO_GRB + NEO_KHZ800);
//   }
// };

#endif