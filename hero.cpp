#include "hero.h"

hero::hero()
{
    this.numBullets = 0;
    this.maxBullets = 3;
    this.bulletVelocity = 10;
    this.body.width = 8;
    this.body.height = 15;
    this.body.center[0] = 400;
    this.body.center[1] = 250;
    this.body.center[2] = 0;
    this.prevPosition.center[0] = 400;
    this.prevPosition.center[1] = 250;
    this.velocity[0] = 0;
    this.velocity[1] = 0;
    this.facing = 1;
}

hero::~hero()
{
    //dtor
}
