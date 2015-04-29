#ifndef ROOM_H
#define ROOM_H
#include <vector>
#include "gameObject.h"


class Room
{
    public:
        int horizontalPosition;
        int verticalPosition;
        int numPlatforms;
        int numBasicEnemies;
        int currentPlatform;
        int currentBasicEnemy;
        std::vector<GameObject*> enemies;
        std::vector<GameObject*> bullet;
        std::vector<GameObject*> objects;

        Room();
        Room(int nPlatform, int numBasic);
        ~Room();
    protected:
    private:
};

#endif // ROOM_H
