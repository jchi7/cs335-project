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
        vector<gameObject*> enemies;
        vector<gameObject*> bullet;
        vector<gameObject*> objects;
        Level(int num, int numBasic);
        virtual ~Level();
    protected:
    private:
};

#endif // LEVEL_H
