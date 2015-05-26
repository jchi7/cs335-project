#include "room.h"

Room::Room()
{
    this->horizontalPosition = 0;
    this->verticalPosition = 0;
    this->numPlatforms = 0;
    this->numSpikes = 0;
    this->numBullet = 0;
    this->numSavePoints = 0;
    this->numBasicEnemies = 0;
    this->numElevators = 0;
    this->currentPlatform = 0;
    this->currentBasicEnemy = 0;
}

Room::Room(int nPlatform, int nSpike, int nSavePoints, int numBasic, int nElevators)
{
    this->numPlatforms = nPlatform;
    this->numSpikes = nSpike;
    this->numSavePoints = nSavePoints;
    this->numBasicEnemies = numBasic;
    this->numElevators = nElevators;

    this->platforms.reserve(nPlatform);
    this->spikes.reserve(nSpike);
    this->savePoints.reserve(nSavePoints);
    this->elevators.reserve(nElevators);
    if (this->numBasicEnemies > 0)
        this->enemies.reserve(numBasic);
    this->currentBasicEnemy = 0;
}

Room::~Room()
{
    for (int i = (int) enemies.size() - 1; i >= 0; i--) {
        delete enemies[i];
    }
    for (int i = (int) bullet.size() - 1; i >= 0; i--) {
        delete bullet[i];
    }
    for (int i = platforms.size() - 1; i >= 0; i--) {
        delete platforms[i];
    }
    for (int i = savePoints.size() - 1; i >= 0; i--) {
        delete savePoints[i];
    }
    for (int i = spikes.size() - 1; i >= 0; i--) {
        delete spikes[i];
    }
    for (int i = elevators.size() - 1; i >= 0; i--) {
        delete elevators[i];
    }
}
