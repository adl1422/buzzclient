#include "../hpp/buttons_callbacks.hpp"
StaticJsonDocument<96> doc;
String jsonData;
extern bool inGame;

void setMessage(String message)
{
    doc["id"] = String(WiFi.macAddress());
    doc["player_id"] = String();
    if (currentMode == START)
    {
        doc["message"] = "Press";
        inGame = true;
    }
    else if (currentMode == GAME)
    {
        doc["message"] = message;
    }
    else
    {
        doc["message"] = "None";
        inGame = false;
    }

    serializeJson(doc, jsonData);
    webSocket.sendTXT(jsonData);
    Serial.println(jsonData);
    jsonData = String();
}
void btnA_click(Button2 &button)
{
    setMessage("a");
}
void btnB_click(Button2 &button)
{
    setMessage("b");
}
void btnC_click(Button2 &button)
{
    setMessage("c");
}
void btnD_click(Button2 &button)
{
    setMessage("d");
}