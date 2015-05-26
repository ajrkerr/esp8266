#include "pixel_strip_config_serializer.h"

String PixelStripConfigSerializerClass::toJSON(PixelStripConfig *pixelStripConfig) {
  JSONRoot *root = new JSONRoot();

  root
    ->integer("type", pixelStripConfig->type)
    ->integer("numPixels", pixelStripConfig->numPixels)
    ->integer("frameLength", pixelStripConfig->frameLength)
    ->integer("stepsPerRainbow", pixelStripConfig->stepsPerRainbow)
    ->object("primaryColor")
      ->integer("red", pixelStripConfig->primaryColor.red)
      ->integer("blue", pixelStripConfig->primaryColor.blue)
      ->integer("green", pixelStripConfig->primaryColor.green)
      ->done()
    ->object("secondaryColor")
      ->integer("red", pixelStripConfig->secondaryColor.red)
      ->integer("blue", pixelStripConfig->secondaryColor.blue)
      ->integer("green", pixelStripConfig->secondaryColor.green)
      ->done();

  String result = root->toString();
  delete root;
  return result;
}

PixelStripConfigSerializerClass PixelStripConfigSerializer;