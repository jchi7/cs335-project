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
        int numBasicEnemies;
        int currentPlatform;
        int currentBasicEnemy;

        std::vector<GameObject*> enemies;
        std::vector<GameObject*> bullet;
        std::vector<GameObject*> platforms;
        std::vector<GameObject*> spikes;

        Room();
        Room(int nPlatform, int nSpikes, int numBasic);
        ~Room();
    protected:
    private:
};

#endif // ROOM_H
