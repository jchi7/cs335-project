#include "hero.h"

hero::hero()
{
    numBullets = 0;
    maxBullets = 3;
    bulletVelocity = 10;
    body.width = 8;
    body.height = 15;
    body.center[0] = 400;
    body.center[1] = 250;
    body.center[2] = 0;
    prevPosition.center[0] = 400;
    prevPosition.center[1] = 250;
    velocity[0] = 0;
    velocity[1] = 0;
    facing = 1;
    jumpInitiated = 0;
    jumpFinished = 0;
    rgb[0] = 200;
    rgb[1] = 200;
    rgb[2] = 200;
}

hero::~hero()
{
    //dtor
}

void hero::update()
{

}
