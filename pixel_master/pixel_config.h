#ifndef PIXEL_CONFIG_H
#define PIXEL_CONFIG_H

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
  unsigned char numPixels;
  unsigned char frameLength;
  unsigned char type;
  // unsigned char stepsPerRainbow;

  Color primaryColor;
  Color secondaryColor;
} PixelConfig;

#endif
