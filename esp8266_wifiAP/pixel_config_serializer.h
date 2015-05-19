#ifndef PIXEL_CONFIG_SERIALIZER_H
#define PIXEL_CONFIG_SERIALIZER_H

#include <Arduino.h>
#include "pixel_config.h"
#include "json.h"

class PixelConfigSerializerClass {
public:
  String toJSON(PixelConfig *pixelConfig);
};

extern PixelConfigSerializerClass PixelConfigSerializer;

#endif