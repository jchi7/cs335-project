#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include "gameObject.h"


class Level
{
    public:
        int horizontalPosition;
        int verticalPosition;
        int numPlatforms;
        int numBasicEnemies;
        int currentPlatform;
        int currentBasicEnemy;
        std::vector<gameObject*> enemies;
        std::vector<gameObject*> bullet;
        std::vector<gameObject*> objects;
        Level();
        Level(int num, int numBasic);
        ~Level();
    protected:
    private:
};

#endif // LEVEL_H
