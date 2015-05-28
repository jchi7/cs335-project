#include "elevator.h"

/* --------------- Elevator Class --------------- */
Elevator::Elevator(float centerX, float lowerY, float upperY, int texture, float vertVelMult)
{
    body.type = RECTANGLE;
    id = ELEVATOR;
    delay = 0;

    if (lowerY > upperY) {
        float temp = lowerY;
        lowerY = upperY;
        upperY = temp;
    }
    vertSpeed = vertVelMult;

    vecMake(centerX, lowerY, body.center);
    vecMake(0, fabs(gravity*2), velocity);
    lowerLimit = lowerY;
    upperLimit = upperY;
    prevPosition[0] = body.center[0];
    prevPosition[1] = body.center[1];

    tex_id = texture;
    textureWidth = 15;  // !!!!! need to merge with game's texture data !!!!!
    textureHeight = 15;
    body.width = textureWidth * 2;
    body.height = textureHeight * 1;
    horizontalTiles = body.width / textureWidth;
    verticalTiles = body.height / textureHeight;
    rgb[0] = 70;
    rgb[1] = 140;
    rgb[2] = 190;
}

Elevator::~Elevator(){}

void Elevator::setUpperLimit(float newLimit)
{
    int liftHeight = newLimit - lowerLimit;
    if (liftHeight <= (body.height * 2)) {
        upperLimit = lowerLimit + (body.height * 2);
    }
    else {
        upperLimit = newLimit;
    }
}

void Elevator::setLowerLimit(float newLimit)
{
    lowerLimit = newLimit;
}

void Elevator::setVertSpeed(float newSpeed)
{
    vertSpeed = fabs(newSpeed);
}

float Elevator::getUpperLimit()
{
    return upperLimit;
}

float Elevator::getLowerLimit()
{
    return lowerLimit;
}

float Elevator::getVertSpeed()
{
    return vertSpeed;
}

void Elevator::movement()
{
    if (delay != 0) {
        delay = (delay + 1) % 20;
        return;
    }
    prevPosition[1] = body.center[1];
    body.center[1] += velocity[1]*vertSpeed;
    if (body.center[1] > upperLimit) {
        body.center[1] = upperLimit;
        velocity[1] = -fabs(velocity[1]);
    }
    else if (body.center[1] < lowerLimit) {
        body.center[1] = lowerLimit;
        velocity[1] = fabs(velocity[1]);
    }
}

string Elevator::debugReport()
{
    ostringstream oss;
    oss
      << "REPORT: Elevator\n"
      << GameObject::debugReport()
      << "  float upperLimit=" << upperLimit << endl
      << "  float lowerLimit=" << lowerLimit << endl
      << "END REPORT: Elevetor\n";
    return oss.str();
}
/* --------------- End Elevator Class --------------- */


