#include "spawnPoint.h"

/* --------------- SpawnPoint Class --------------- */
SpawnPoint::SpawnPoint(int x, int y, ObjectType type)
{
    body.type = RECTANGLE;
    id = type;
    body.center[0] = x;
    body.center[1] = y;
    body.center[2] = 0;
}

SpawnPoint::~SpawnPoint()
{
    //dtor
}

void SpawnPoint::update()
{
    //update
}
/* --------------- End SpawnPoint Class --------------- */
