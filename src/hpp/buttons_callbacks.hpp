#ifndef BTN_CALLBACKS_HPP
#define BTN_CALLBACKS_HPP
#include <Button2.h>
#include <Adafruit_NeoPixel.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "../hpp/pixels.hpp"
#include "../hpp/defines.h"

extern Adafruit_NeoPixel pixels;
extern WebSocketsClient webSocket;
extern Mode currentMode;

void btnA_click(Button2 &button);
void btnB_click(Button2 &button);
void btnC_click(Button2 &button);
void btnD_click(Button2 &button);



#endif