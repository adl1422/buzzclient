#include <ESP8266WiFi.h>
#include <Button2.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include "src/hpp/buttons_callbacks.hpp"
#include "src/hpp/pixels.hpp"
#include "src/hpp/defines.h"

WebSocketsClient webSocket;
Adafruit_NeoPixel pixels(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);
bool started = false;
Button2 btnA(BTN_A, INPUT_PULLUP, 50);
Button2 btnB(BTN_B, INPUT_PULLUP, 50);
Button2 btnC(BTN_C, INPUT_PULLUP, 50);
Button2 btnD(BTN_D, INPUT_PULLUP, 50);


Mode currentMode = NONE;

String getMacAddress()
{
    return String(WiFi.macAddress());
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    StaticJsonDocument<96> doc;
    DeserializationError error = deserializeJson(doc, payload);
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[WSc] Disconnected!\n");
        setColor(ORANGE); //Déconnecté du websocket -> orange
        break;
    case WStype_CONNECTED:
    {
        Serial.printf("[WSc] Connected to url: %s\n", payload);
        setColor(GREEN); //Connecté au websocket -> vert
        // webSocket.sendTXT("{\"message\": \"Connect\", \"id\":\"" + getMacAddress() + "\"}");
    }
    break;
    case WStype_TEXT:
        // Serial.printf("[WSc] get text: %s\n", payload);
        Serial.println("[WSc] get text : ");

        if (error)
        {
            Serial.print("Erreur deserialisation : ");
            Serial.println(error.c_str());
            // return;
        }
        Serial.print("id : ");
        Serial.println(doc["id"].as<String>());
        Serial.print("message : ");
        Serial.println(doc["message"].as<String>());
        if (doc["id"] == "admin")
        {
            if (doc["message"] == "start")
            {
                currentMode = START;
            }
            if (doc["message"] == "game")
            {
                currentMode = GAME;
            }
            else if (doc["message"] == "reset")
            {
                currentMode = NONE;
            }
        }
        Serial.print("Current mode : ");
        Serial.println(currentMode);
        // webSocket.sendTXT("{\"message\": \"How are you ?\"}");
        // send message to server
        // webSocket.sendTXT("message here");
        break;
    case WStype_BIN:
        Serial.printf("[WSc] get binary length: %u\n", length);
        hexdump(payload, length);

        // send data to server
        // webSocket.sendBIN(payload, length);
        break;
    case WStype_PING:
        // pong will be send automatically
        Serial.printf("[WSc] get ping\n");
        break;
    case WStype_PONG:
        // answer to a ping we send
        Serial.printf("[WSc] get pong\n");
        break;
    }
}

void click(Button2 &btn)
{
    for (size_t i = 0; i < NUMPIXELS; i++)
    {
        if (i % 4 == 0)
        {
            pixels.setPixelColor(i, pixels.Color(0, 255, 0));
            pixels.show();
        }
    }
    Serial.println("Button pressed");
    StaticJsonDocument<64> doc;
    doc["id"] = getMacAddress();
    if (currentMode == START)
    {
        doc["message"] = "Press";
    }
    else if (currentMode == GAME)
    {
        doc["message"] = "1";
    }
    else
    {
        doc["message"] = "None";
    }

    String jsonData;
    serializeJson(doc, jsonData);
    Serial.println(jsonData);
    webSocket.sendTXT(jsonData);
    // webSocket.sendTXT("{\"message\": \"Press\", \"id\":\"" + getMacAddress() + "\"}");
}

void long_click(Button2 &btn)
{
    StaticJsonDocument<64> doc;
    doc["id"] = getMacAddress();
    if (currentMode == START)
    {
        doc["message"] = "long Press";
    }
    else if (currentMode == GAME)
    {
        doc["message"] = "2";
    }

    String jsonData;
    serializeJson(doc, jsonData);
    webSocket.sendTXT(jsonData);
}

void setup()
{
    Serial.begin(115200);
    pixels.begin();
    pixels.setBrightness(25);

    WiFi.begin("WLanHome", "456Pazertyuio");
    int count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        
        if (count % 2 == 0)
        {
            setColor(RED);
        }
        else
        {
            setColor(BLACK);
        }
        ++count;
        Serial.print('.');
        delay(500);
    }
    setColor(ORANGE); //On est connecté au wifi -> orange

    Serial.print("Ip : ");
    Serial.println(WiFi.localIP());
    webSocket.begin("192.168.1.11", 8080, "/ws/pads/");

    // event handler
    webSocket.onEvent(webSocketEvent);
    btnA.setClickHandler(btnA_click);
    btnB.setClickHandler(btnB_click);
    btnC.setClickHandler(btnC_click);
    btnD.setClickHandler(btnD_click);
}

void loop()
{

    btnA.loop();
    btnB.loop();
    btnC.loop();
    btnD.loop();
    webSocket.loop();
}