#include "../hpp/buttons_callbacks.hpp"

void btnA_click(Button2 &button)
{
  
}
void btnB_click(Button2 &button)
{
    Serial.println(currentMode);
    setColor(255,0,255);
}
void btnC_click(Button2 &button)
{
    setColor(0,255,0);
}
void btnD_click(Button2 &button)
{
  
    setColor(0,0,255);
}