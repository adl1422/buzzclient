#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include "src/hpp/buttons_callbacks.hpp"
#include "src/hpp/pixels.hpp"
#include "src/hpp/defines.h"
#include "src/hpp/color.hpp"
#include "src/hpp/ButtonBase.hpp"

WebSocketsClient webSocket;
Adafruit_NeoPixel pixels(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);
bool started = false;

ButtonBase btnA(BTN_A);
ButtonBase btnB(BTN_B);
ButtonBase btnC(BTN_C);
ButtonBase btnD(BTN_D);

Color colors[5];

bool inGame = false;
Mode currentMode = NONE;
Color myColor;
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    StaticJsonDocument<256> doc;

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
    }
    break;
    case WStype_TEXT:
 
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
                    setColor(BLACK);
                    delay(500);
                    setColor(myColor.r(), myColor.g(), myColor.b());
                }
            }
            else if (doc["message"] == "reset")
            {
                currentMode = NONE;
                setColor(GREEN);
                inGame = false;
            }

            else if (doc["message"] == "new_quest")
            {
                if (inGame)
                {
                    currentMode = GAME;
                    setColor(myColor.r(), myColor.g(), myColor.b());
                }
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
                        myColor = Color("myColor",doc["color_r"].as<uint8_t>(),doc["color_g"].as<uint8_t>(),doc["color_b"].as<uint8_t>() );
                        setColor(myColor.r(), myColor.g(), myColor.b());
                    }
                }
                else
                {
                    if (doc["message"] == "faster")
                    {
                        if (inGame)
                            setColor(RED);
                    }
                }
            }
        }


        break;
    case WStype_BIN:
        Serial.printf("[WSc] get binary length: %u\n", length);
        hexdump(payload, length);

        break;
    case WStype_PING:
        // pong will be send automatically
        // Serial.printf("[WSc] get ping\n");
        break;
    case WStype_PONG:
        // answer to a ping we send
        // Serial.printf("[WSc] get pong\n");
        break;
    }
}

void setup()
{
    Serial.begin(115200);
    // /!\ Maintenir l'ordre des couleurs car lié aux idx du tableau client 

    colors[0] = Color("RedRuby", REDRUBY);
    colors[1] = Color("BlueLavender", BLUELAVENDER);
    colors[2] = Color("Turquoise", TURQUOISE);
    colors[3] = Color("Purple", PURPLE);
    colors[4] = Color("PurpleDark", PURPLEDARK);
    pixels.begin();
    pixels.setBrightness(25);

    WiFi.begin(SSID, PASS);
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
    webSocket.begin(HOST, PORT, "/ws/pads/");

    webSocket.onEvent(webSocketEvent);

    btnA.on_click(btnA_click);
    btnB.on_click(btnB_click);
    btnC.on_click(btnC_click);
    btnD.on_click(btnD_click);
}

void loop()
{

    btnA.read();
    btnB.read();
    btnC.read();
    btnD.read();
    webSocket.loop();
}

