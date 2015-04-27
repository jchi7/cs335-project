#include <cstring>
#include "platform.h"

/* --------------- Platform Class --------------- */
Platform::Platform(int width, int height, int x, int y, const char * ptype)
{
    body.center[0] = x;
    body.center[1] = y;
    body.center[2] = 0;
    body.width = width;
    body.height = height;
    rgb[0] = 90;
    rgb[1] = 140;
    rgb[2] = 90;

    if (strcmp(ptype,"SPIKE") == 0) {
        type = SPIKE;
    }
    else {
        type = GROUND;
    }
}

Platform::~Platform()
{
    //dtor
}

void Platform::update()
{
}
/* --------------- End Platform Class --------------- */


