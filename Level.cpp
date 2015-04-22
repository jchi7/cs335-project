#include "Level.h"
#include <iostream>

Level::Level()
{

}

Level::Level(int num, int numBasic)
{
    this->numPlatforms = num;
    std::cout << "There again1" << std::endl;
    this->objects.reserve(num);
    std::cout << "There again2" << std::endl;
    this->numBasicEnemies = numBasic;
    std::cout << "There again3" << std::endl;
    if (this->numBasicEnemies > 0)
        this->enemies.reserve(numBasic);
    std::cout << "There again4" << std::endl;
    this->currentBasicEnemy = 0;
    std::cout << "There again5" << std::endl;
}

Level::~Level()
{
    //dtor
}
