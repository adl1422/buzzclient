#include "../hpp/ButtonBase.hpp"

ButtonBase::ButtonBase(unsigned int pin) : m_pin(pin)
{
    pinMode(m_pin, INPUT_PULLUP);
}

void ButtonBase::on_click(CallbackFunction cb)
{
    cb_click = cb;
}
void ButtonBase::on_long_click(CallbackFunction cb)
{
    cb_long_click = cb;
}

void ButtonBase::read()
{
    int reading = digitalRead(m_pin);

    if (reading == LOW)
    {
        // Si c'est le premier click
        if (m_lastPressed != 0)
        {
            m_totalTime += millis() - m_lastPressed;
            // On attend que le press dure au moins une seconde
            if (m_totalTime > LONG_PRESS_DELAY && !m_hasLongPressed)
            {
                // long press
                m_hasLongPressed = true;
                if (cb_long_click != NULL)
                    cb_long_click();
            }
        }
        m_lastPressed = millis();
    }
    else
    {
        m_totalTime = 0;
        m_lastPressed = 0;
    }
    if (m_lastBtnState != reading)
    {
        m_debounceTime = millis();
    }

    // On se debarrasse de l'effet de rebond mecanique
    if (millis() - m_debounceTime > DEBOUNCE_DELAY)
    {
        if (reading != m_buttonState)
        {
            m_buttonState = reading;
            if (m_buttonState == HIGH)
            {
                if (m_hasLongPressed)
                    m_hasLongPressed = false;
                else
                {
                    if (cb_click != NULL)
                        cb_click();
                }
            }
        }
    }
    m_lastBtnState = reading;
}
