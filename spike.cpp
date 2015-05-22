#include "spike.h"

/* --------------- Spike Class --------------- */
Spike::Spike(Vec p[3], ShapeOrientation orientation)
{
    body.type = TRIANGLE;
    id = SPIKE;
    this->body.orientation = orientation;
    vecCopy(p[0], body.corners[0]);
    vecCopy(p[1], body.corners[1]);
    vecCopy(p[2], body.corners[2]);

    vecMake(
      (float)(body.corners[0][0] + body.corners[1][0] + body.corners[2][0])/3.0,
      (float)(body.corners[0][1] + body.corners[1][1] + body.corners[2][1])/3.0,
      body.center);
    vecCopy(body.center, prevPosition);
    vecZero(velocity);

    rgb[0] = 170;
    rgb[1] = 90;
    rgb[2] = 90;
}

Spike::Spike(GameObject * source)
{
    body.type = TRIANGLE;
    id = SPIKE;
    body.orientation = source->body.orientation;
    for (int i = 0; i < 3; i++) {
        vecCopy(source->body.corners[i], body.corners[i]);
        rgb[i] = source->rgb[i];
    }
    vecCopy(source->body.center, body.center);
    vecCopy(source->prevPosition, prevPosition);
    vecCopy(source->velocity, velocity);
}

Spike::~Spike()
{
    //dtor
}

void Spike::update()
{
}

string Spike::debugReport()
{
  ostringstream oss;
  oss
    << "REPORT: Spike\n"
    << GameObject::debugReport()
    << "END REPORT: Spike\n";
  return oss.str();
}
/* --------------- End Spike Class --------------- */


