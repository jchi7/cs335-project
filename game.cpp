#include "game.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

game::game(Level *** newlevel)
{
    this->leftPressed = 0;
    this->rightPressed = 0;
    this->shootPressed = 0;
    this->level = newlevel;
    this->currentHorizontalLevel = 3;
    this->currentVerticalLevel = 1;
    this->state = PLAYING;
}

game::~game()
{
    delete hero;
    //dtor
}
void game::checkRoom()
{
    if (hero->body.center[0] > WINDOW_WIDTH){
        currentHorizontalLevel++;
        hero->body.center[0] = 0 + hero->body.width;
    }
    if (hero->body.center[0] < 0){
        currentHorizontalLevel--;
        hero->body.center[0] = WINDOW_WIDTH - hero->body.width;
    }
    if (hero->body.center[1] > WINDOW_HEIGHT){
        currentVerticalLevel++;
        hero->body.center[1] = 0 + hero->body.height;
    }
    if (hero->body.center[1] < 0){
        currentVerticalLevel--;
        hero->body.center[1] = WINDOW_HEIGHT - hero->body.height;
    }
}
