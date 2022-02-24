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

bool inGame = false;
Mode currentMode = NONE;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    StaticJsonDocument<96> doc;
    DeserializationError error = deserializeJson(doc, payload);
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[WSc] Disconnected!\n");
        setColor(ORANGE); // Déconnecté du websocket -> orange
        break;
    case WStype_CONNECTED:
    {
        Serial.printf("[WSc] Connected to url: %s\n", payload);
        setColor(GREEN); // Connecté au websocket -> vert
        // webSocket.sendTXT("{\"message\": \"Connect\", \"id\":\"" + getMacAddress() + "\"}");
    }
    break;
    case WStype_TEXT:
        // Serial.printf("[WSc] get text: %s\n", payload);
        // Serial.println("[WSc] get text : ");

        if (error)
        {
            Serial.print("Erreur deserialisation : ");
            Serial.println(error.c_str());
            // return;
        }

        if (doc["id"] == "admin")
        {
            if (doc["message"] == "start")
            {
                currentMode = START;
                setColor(BLACK);
            }
            if (doc["message"] == "game")
            {
                if (inGame)
                {
                    currentMode = GAME;
                    setColor(BLUE);
                }
            }
            else if (doc["message"] == "reset")
            {
                currentMode = NONE;
                setColor(GREEN);
                inGame = false;
            }
            else
            {
                if (doc["player_id"] == String(WiFi.macAddress()))
                {
                    if (doc["message"] == "good" || doc["message"] == "faster")
                    {

                        setColor(GREEN);
                    }
                    else if (doc["message"] == "bad")
                    {
                        setColor(RED);
                    }
                    else if (doc["message"] == "OK")
                    {
                        setColor(YELLOW);
                    }
                }
                else
                {
                    if(doc["message"] == "faster")
                    {
                        if(inGame)
                            setColor(RED);
                    }
                }
                
            }
        }

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
    setColor(ORANGE); // On est connecté au wifi -> orange

    Serial.print("Ip : ");
    Serial.println(WiFi.localIP());
    webSocket.begin("192.168.1.9", 80, "/ws/pads/");

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