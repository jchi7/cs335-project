#ifndef GAME_H
#define GAME_H
#include "gameObject.h"
#include "Level.h"

class game
{
    public:
        game(gameObject* newhero, Level*** newlevel);
        virtual ~game();
        Level* level;
        gameObject* hero;
    protected:
    private:
};

#endif // GAME_H
