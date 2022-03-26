#include "../hpp/buttons_callbacks.hpp"

StaticJsonDocument<256> doc;
String jsonData;
extern bool inGame;
void setMessage(String message)
{

    doc["id"] = String(WiFi.macAddress());
    doc["player_id"] = String();
    doc["score"] = 0;
    doc["color_r"] = 0;
    doc["color_g"] = 0;
    doc["color_b"] = 0;
    doc["game_mode"] = String();
    doc["player_number"] = 0;

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
    jsonData = String();
}
void btnA_click()
{
    setMessage("a");
}
void btnB_click()
{
    setMessage("b");
}
void btnC_click()
{
    setMessage("c");
}
void btnD_click()
{
    setMessage("d");
}