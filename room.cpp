#include "room.h"

Room::Room()
{
    this->horizontalPosition = 0;
    this->verticalPosition = 0;
    this->numPlatforms = 0;
    this->numSpikes = 0;
    this->numSavePoints = 0;
    this->numBasicEnemies = 0;
    this->currentPlatform = 0;
    this->currentBasicEnemy = 0;
}

Room::Room(int nPlatform, int nSpike, int nSavePoints, int numBasic)
{
    this->numPlatforms = nPlatform;
    this->platforms.reserve(nPlatform);
    this->spikes.reserve(nSpike);
    this->savePoints.reserve(nSavePoints);
    this->numBasicEnemies = numBasic;
    if (this->numBasicEnemies > 0)
        this->enemies.reserve(numBasic);
    this->currentBasicEnemy = 0;
}

Room::~Room()
{
    for (unsigned int i = 0; i < enemies.size(); i++) {
        delete enemies[i];
    }
    for (unsigned int i = 0; i < bullet.size(); i++) {
        delete bullet[i];
    }
    for (unsigned int i = 0; i < platforms.size(); i++) {
        delete platforms[i];
    }
    for (unsigned int i = 0; i < savePoints.size(); i++) {
        delete savePoints[i];
    }
    for (unsigned int i = 0; i < spikes.size(); i++) {
        delete spikes[i];
    }
}
