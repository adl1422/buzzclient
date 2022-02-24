#include "../hpp/pixels.hpp"

void setColor(uint8_t r, uint8_t g, uint8_t b)
{

    pixels.clear();
    for (size_t i = 0; i < NUMPIXELS; i++)
    {
        if (i % MODULOLEDS == 0)
        {
            pixels.setPixelColor(i, pixels.Color(r, g, b));
            pixels.show();
        }
    }
}

