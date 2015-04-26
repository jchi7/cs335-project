#ifndef GAME_H
#define GAME_H
#include "gameObject.h"
#include "Level.h"
#include "hero.h"

//enum GameState {PLAYING, EXIT_GAME, MAIN_MENU, LEVEL_EDITOR}; 

class game
{
    public:
        int leftPressed;
        int rightPressed;
        int shootPressed;
        int currentHorizontalLevel;
        int currentVerticalLevel;
        Level*** level;
        Hero* hero;

        GameState state;

        game(Level*** newlevel);
        ~game();

        void checkRoom();
    protected:
    private:
};

#endif // GAME_H
