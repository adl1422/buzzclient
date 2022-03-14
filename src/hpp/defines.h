#ifndef DEFINES_H
#define DEFINES_H
//Wifi
#define SSID "WLanHome"
#define PASS "456Pazertyuio"

//WebClient
#define HOST "192.168.1.11"
#define PORT 8080

//WS2812b
#define NUMPIXELS 16
#define PIXELPIN 2
//modulo pour 1 led/ 4
#define MODULOLEDS 4
//Pins boutons
#define BTN_A 0
#define BTN_B 12
#define BTN_C 14
#define BTN_D 4

//Couleurs

#define RED 255,0,0
#define GREEN 0,255,0
#define BLUE 0,0,255
#define ORANGE 255,127,0
#define BLACK 0,0,0
#define YELLOW 255,255,0
#define BLUELAVENDER 177, 145, 255
#define REDRUBY 163, 0, 21
#define PURPLE 255, 0, 255
#define PURPLEDARK 94, 35, 157
#define TURQUOISE 65, 234, 212


enum Mode
{
    NONE,
    START,
    GAME
};


#endif