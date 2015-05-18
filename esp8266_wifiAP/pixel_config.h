#ifndef PIXEL_CONFIG_H
#define PIXEL_CONFIG_H

#include <Arduino.h>

typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} Color;

enum Animation { 
  PIXEL_SOLID, 
  PIXEL_TRACER, 
  PIXEL_RAINBOW,
  PIXEL_FLAME
};

typedef struct {
  uint16_t numPixels;
  uint16_t frameLength;
  uint16_t stepsPerRainbow;

  byte type;

  Color primaryColor;
  Color secondaryColor;
} PixelConfig;

#endif
