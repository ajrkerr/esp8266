#ifndef PIXEL_STRIP_CONFIG_SERIALIZER_H
#define PIXEL_STRIP_CONFIG_SERIALIZER_H

#include <Arduino.h>
#include "pixel_strip_config.h"
#include "json.h"

class PixelStripConfigSerializerClass {
public:
  String toJSON(PixelStripConfig *pixelStripConfig);
};

extern PixelStripConfigSerializerClass PixelStripConfigSerializer;

#endif