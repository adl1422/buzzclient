#ifndef BUTTONBASE_HPP
#define BUTTONBASE_HPP

#include <Arduino.h>
#include "ButtonOptions.hpp"

class ButtonBase
{
private:
    typedef void (*CallbackFunction)();
    bool m_hasLongPressed = false;
    unsigned int m_pin;

    int m_lastBtnState;
    unsigned long m_debounceTime;
    int m_buttonState = HIGH;
    unsigned long m_lastPressed = 0;
    unsigned long m_totalTime = 0;
    CallbackFunction cb_click = NULL;
    CallbackFunction cb_long_click = NULL;

public:
    ButtonBase(unsigned int pin);
    ~ButtonBase(){};
    bool isPressed();
    void on_click(CallbackFunction cb);
    void on_long_click(CallbackFunction cb);
    void read();
};

#endif