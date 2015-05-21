#include "pixel_config_serializer.h"

String PixelConfigSerializerClass::toJSON(PixelConfig *pixelConfig) {
  JSONRoot *root = new JSONRoot();

  root
    ->integer("type", pixelConfig->type)
    ->integer("numPixels", pixelConfig->numPixels)
    ->integer("frameLength", pixelConfig->frameLength)
    ->integer("stepsPerRainbow", pixelConfig->stepsPerRainbow)
    ->object("primaryColor")
      ->integer("red", pixelConfig->primaryColor.red)
      ->integer("blue", pixelConfig->primaryColor.blue)
      ->integer("green", pixelConfig->primaryColor.green)
      ->done()
    ->object("secondaryColor")
      ->integer("red", pixelConfig->secondaryColor.red)
      ->integer("blue", pixelConfig->secondaryColor.blue)
      ->integer("green", pixelConfig->secondaryColor.green)
      ->done();

  String result = root->toString();
  delete root;
  return result;
}

PixelConfigSerializerClass PixelConfigSerializer;