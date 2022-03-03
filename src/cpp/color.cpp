#include "../hpp/color.hpp"

Color::Color()
{
}
Color::Color(String name, uint8_t r, uint8_t g, uint8_t b) : _r(r), _g(g), _b(b), Name(name)
{
}

uint8_t Color::r()
{
    return _r;
}
uint8_t Color::g()
{
    return _g;
}
uint8_t Color::b()
{
    return _b;
}
