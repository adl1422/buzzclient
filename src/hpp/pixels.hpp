#ifndef PIXELS_HPP
#define PIXELS_HPP
#include <Adafruit_NeoPixel.h>
#include "../hpp/defines.h"
extern Adafruit_NeoPixel pixels;

void setColor(uint8_t r, uint8_t g, uint8_t b);

#endif