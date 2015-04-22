#include "game.h"

game::game(Level*** newlevel)
{
    this->leftPressed = 0;
    this->rightPressed = 0;
    this->shootPressed = 0;
    this->level = newlevel;
    this->currentHorizontalLevel = 10;
    this->currentVerticalLevel = 2;
}

game::~game()
{
    delete hero;
    //dtor
}
