#include "Level.h"

Level::Level(int num, int numBasic)
{
    this.numPlatforms = num;
    this.objects.reserve(num);
    this.numBasicEnemies = numBasic;
    if (this.numBasicEnemies > 0)
        this.enemies.reserve(numBasic);
    this.currentBasicEnemy = 0;
}

Level::~Level()
{
    //dtor
}
