#ifndef GAME_H
#define GAME_H
#include "gameObject.h"
#include "Level.h"
#include "hero.h"


class game
{
    public:
        int leftPressed;
        int rightPressed;
        int shootPressed;
        int currentHorizontalLevel;
        int currentVerticalLevel;
        Level*** level;
        hero* hero;

        game(Level*** newlevel);
        ~game();

        void checkRoom();
    protected:
    private:
};

#endif // GAME_H
