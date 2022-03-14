#ifndef BTN_CALLBACKS_HPP
#define BTN_CALLBACKS_HPP
#include <Adafruit_NeoPixel.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "../hpp/pixels.hpp"
#include "../hpp/defines.h"

extern Adafruit_NeoPixel pixels;
extern WebSocketsClient webSocket;
extern Mode currentMode;

void btnA_click();
void btnB_click();
void btnC_click();
void btnD_click();



#endif