#include "savePoint.h"

/* --------------- SavePoint Class --------------- */
SavePoint::SavePoint(int width, int height, int x, int y)
{
    body.type = RECTANGLE;
    id = SAVEPOINT;
    body.center[0] = x;
    body.center[1] = y;
    body.center[2] = 0;
    body.width = width;
    body.height = height;
    rgb[0] = 240;
    rgb[1] = 240;
    rgb[2] = 0;
}

SavePoint::~SavePoint()
{
    //dtor
}

void SavePoint::update()
{
}
/* --------------- End SavePoint Class --------------- */


