#include "game.h"

game::game(gameObject* newhero, Level*** newlevel)
{
    this->level = newlevel;
    this->hero = newhero;
}

game::~game()
{
    //dtor
}
