#include "platform.h"

/* --------------- Platform Class --------------- */
Platform::Platform(int width, int height, int x, int y, int tex)
{
    body.type = RECTANGLE;
    id = PLATFORM;
    tex_id = tex;

    vecMake(x,y,body.center);
    vecCopy(body.center, prevPosition);
    vecZero(velocity);

    body.width = width;
    body.height = height;
    textureWidth = 15;     // !!!!! need to merge with game's texture data !!!!!
    textureHeight = 15;
    horizontalTiles = width / textureWidth;
    verticalTiles = height / textureHeight;
    rgb[0] = 90;
    rgb[1] = 140;
    rgb[2] = 90;
}

Platform::Platform(GameObject * source)
{
    body.type = RECTANGLE;
    id = PLATFORM;

    tex_id = source->tex_id;
    vecCopy(source->body.center, body.center);
    vecCopy(source->prevPosition, prevPosition);
    vecCopy(source->velocity, velocity);

    body.width = source->body.width;
    body.height = source->body.height;
    textureWidth = source->textureWidth;
    textureHeight = source->textureHeight;
    horizontalTiles = source->horizontalTiles;
    verticalTiles = source->verticalTiles;
    for (int i = 0; i < 3; i++) {
        rgb[i] = source->rgb[i];
    }
}

Platform::~Platform()
{
    //dtor
}

void Platform::update()
{
}

string Platform::debugReport()
{
  ostringstream oss;
  oss
    << "REPORT: Platform\n"
    << GameObject::debugReport()
    << "END REPORT: Platform\n";
  return oss.str();
}
/* --------------- End Platform Class --------------- */


