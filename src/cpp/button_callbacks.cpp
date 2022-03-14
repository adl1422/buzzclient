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
void btnA_click()
{
    setMessage("a");
    // Serial.println("a clicked");
}
void btnB_click()
{
    setMessage("b");
    // Serial.println("b clicked");
}
void btnC_click()
{
    setMessage("c");
}
void btnD_click()
{
    setMessage("d");
}