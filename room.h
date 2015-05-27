#ifndef ROOM_H
#define ROOM_H

#include "gameObject.h"
#include "basicEnemy.h"
#include "shooterEnemy.h"
#include "spawnPoint.h"
#include "elevator.h"

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
        int numElevators;
        int currentPlatform;
        int currentBasicEnemy;

        std::vector<GameObject*> enemies;
        std::vector<GameObject*> bullet;
        std::vector<GameObject*> platforms;
        std::vector<GameObject*> spikes;
        std::vector<GameObject*> savePoints;
        std::vector<GameObject*> spawnPoints;
        std::vector<Elevator*> elevators;

        Room();
        Room(int nPlatform, int nSpikes, int nSavePoints, int numBasic, int nElevators);
        ~Room();
        void respawn();
    protected:
    private:
};

#endif // ROOM_H
