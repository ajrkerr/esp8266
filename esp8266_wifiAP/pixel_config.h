#ifndef PIXEL_CONFIG_H
#define PIXEL_CONFIG_H

typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} Color;

typedef struct {
  unsigned int numPixels;
  Color primaryColor;
} PixelConfig;

#endif
