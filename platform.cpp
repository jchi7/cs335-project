#include "platform.h"

/* --------------- Platform Class --------------- */
Platform::Platform(int width, int height, int x, int y)
{
    body.type = RECTANGLE;
    id = PLATFORM;
    body.center[0] = x;
    body.center[1] = y;
    body.center[2] = 0;
    body.width = width;
    body.height = height;
    textureWidth = 15;
    textureHeight = 15;
    horizontalTiles = width / textureWidth;
    verticalTiles = height / textureHeight;
    rgb[0] = 90;
    rgb[1] = 140;
    rgb[2] = 90;
}

Platform::~Platform()
{
    //dtor
}

void Platform::update()
{
}
/* --------------- End Platform Class --------------- */


