#include <ESP8266WiFi.h>
#include <Button2.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include "src/hpp/buttons_callbacks.hpp"
#include "src/hpp/pixels.hpp"
#include "src/hpp/defines.h"
#include "src/hpp/color.hpp"

WebSocketsClient webSocket;
Adafruit_NeoPixel pixels(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);
bool started = false;
Button2 btnA(BTN_A, INPUT_PULLUP, 50);
Button2 btnB(BTN_B, INPUT_PULLUP, 50);
Button2 btnC(BTN_C, INPUT_PULLUP, 50);
Button2 btnD(BTN_D, INPUT_PULLUP, 50);

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
        // webSocket.sendTXT("{\"message\": \"Connect\", \"id\":\"" + getMacAddress() + "\"}");
    }
    break;
    case WStype_TEXT:
        // Serial.printf("[WSc] get text: %s\n", payload);

        // Serial.println("[WSc] get text : ");
        Serial.print("I am");
        Serial.println(String(WiFi.macAddress()));
        Serial.print("id : ");
        Serial.print(doc["id"].as<String>());
        Serial.print(", player_id : ");
        Serial.print(doc["player_id"].as<String>());
        Serial.print(", message : ");
        Serial.print(doc["message"].as<String>());
        Serial.print(", score : ");
        Serial.print(doc["score"].as<int>());
        Serial.print(", r : ");
        Serial.print(doc["color_r"].as<uint8_t>());
        Serial.print(", g : ");
        Serial.print(doc["color_g"].as<uint8_t>());
        Serial.print(", b : ");
        Serial.println(doc["color_b"].as<uint8_t>());
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
                        // Serial.print("Couleur r : ");
                        // Serial.print(doc["color_r"].as<int>());
                        // Serial.print(" Couleur g : ");
                        // Serial.print(doc["color_g"].as<int>());
                        // Serial.print(" Couleur b : ");
                        // Serial.print(doc["color_b"].as<int>());
                        // Serial.println();
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