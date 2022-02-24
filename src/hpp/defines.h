#ifndef DEFINES_H
#define DEFINES_H

#define NUMPIXELS 16
#define PIXELPIN 2
#define MODULOLEDS 4
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

enum Mode
{
    NONE,
    START,
    GAME
};

#endif