#ifndef ROOM_H
#define ROOM_H

#include "gameObject.h"


class Room
{
    public:
        int horizontalPosition;
        int verticalPosition;
        int numPlatforms;
        int numSpikes;
        int numBullet;
        int numSavePoints;
        int numBasicEnemies;
        int currentPlatform;
        int currentBasicEnemy;

        std::vector<GameObject*> enemies;
        std::vector<GameObject*> bullet;
        std::vector<GameObject*> platforms;
        std::vector<GameObject*> spikes;
        std::vector<GameObject*> savePoints;

        Room();
        Room(int nPlatform, int nSpikes, int nSavePoints, int numBasic);
        ~Room();
    protected:
    private:
};

#endif // ROOM_H
