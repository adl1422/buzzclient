#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

WebSocketClient websocket;
WiFiClient client;
char path[] = "ws/chat/pads/";
char host[] = "192.168.1.9";
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
    if (client.connect(host, 8000))
    {
        Serial.println("Connected");
    }
    else
    {
        Serial.println("Connection failed.");
        while (1)
        {
            delay(1);
        }
    }

    websocket.path = path;
    websocket.host = host;
    if (websocket.handshake(client))
    {
        Serial.println("Handshake successful");
    }
    else
    {
        Serial.println("Handshake failed.");
        // while (1)
        // {
        //    delay(1);
        // }
    }
}

void loop()
{
    String data;

    if (client.connected())
    {

        websocket.getData(data);
        if (data.length() > 0)
        {
            Serial.print("Received data: ");
            Serial.println(data);
        }

        // capture the value of analog 1, send it along
        pinMode(1, INPUT);
        data = String(analogRead(1));

        websocket.sendData(String("{\"message\": \"hello\"}"));
    }
    else
    {
        Serial.println("Client disconnected.");
        // while (1)
        // {
        //     delay(1);
        // }
    }

    // wait to fully let the client disconnect
    delay(3000);
}