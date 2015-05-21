#include "spike.h"

/* --------------- Spike Class --------------- */
Spike::Spike(Vec p[3],ShapeOrientation orientation)
{
    body.type = TRIANGLE;
    id = SPIKE;
    this->body.orientation = orientation;
    vecCopy(p[0], body.corners[0]);
    vecCopy(p[1], body.corners[1]);
    vecCopy(p[2], body.corners[2]);    
    rgb[0] = 170;
    rgb[1] = 90;
    rgb[2] = 90;
}

Spike::~Spike()
{
    //dtor
}

void Spike::update()
{
}
/* --------------- End Spike Class --------------- */


