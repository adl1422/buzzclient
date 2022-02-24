#include <ESP8266WiFi.h>
#include <Button2.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

WebSocketsClient webSocket;
bool started = false;
Button2 btn(13, INPUT_PULLUP, 30);

enum Mode
{
    NONE,
    START,
    GAME
};

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
        break;
    case WStype_CONNECTED:
    {
        Serial.printf("[WSc] Connected to url: %s\n", payload);

        // send message to server when Connected
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
    Serial.println("Button pressed");
    StaticJsonDocument<64> doc;
    doc["id"] = getMacAddress();
    if(currentMode == START)
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
        if(currentMode == START)
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
    WiFi.begin("WLanHome", "456Pazertyuio");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(500);
    }

    Serial.print("Ip : ");
    Serial.println(WiFi.localIP());
    webSocket.begin("192.168.1.10", 8080, "/ws/pads/");

    // event handler
    webSocket.onEvent(webSocketEvent);
    btn.setClickHandler(click);
    btn.setLongClickHandler(long_click);
}

void loop()
{
    
    btn.loop();
    webSocket.loop();

}