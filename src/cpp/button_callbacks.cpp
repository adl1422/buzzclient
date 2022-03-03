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
    // Serial.println(jsonData);
    // Serial.print("Mode : ");
    // Serial.println(currentMode);
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