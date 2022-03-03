#ifndef COLOR_HPP
#define COLOR_HPP
#include <Arduino.h>
class Color
{
private:
    uint8_t _r;
    uint8_t _g;
    uint8_t _b;

public:
 
    Color();
    Color(String name, uint8_t r, uint8_t g, uint8_t b);

    String Name;
    uint8_t r();
    uint8_t g();
    uint8_t b();
    ~Color() {}
};

#endif