#include "room.h"
#include <iostream>

Room::Room()
{
    this->horizontalPosition = 0;
    this->verticalPosition = 0;
    this->numPlatforms = 0;
    this->numBasicEnemies = 0;
    this->currentPlatform = 0;
    this->currentBasicEnemy = 0;
}

Room::Room(int nPlatform, int numBasic)
{
    this->numPlatforms = nPlatform;
    this->objects.reserve(nPlatform);
    this->numBasicEnemies = numBasic;
    if (this->numBasicEnemies > 0)
        this->enemies.reserve(numBasic);
    this->currentBasicEnemy = 0;
}

Room::~Room()
{
    //dtor
}
