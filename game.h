#ifndef GAME_H
#define GAME_H
#include <vector>
#include "gameObject.h"
#include "room.h"
#include "platform.h"
#include "hero.h"


class Game
{
    public:
        int leftPressed;
        int rightPressed;
        int shootPressed;

        // position of current room:
        int currentHorizontalLevel;
        int currentVerticalLevel;

        // level dimensions:
        int totalHorizontal;  //number of columns
        int totalVertical;  // number of rows

        std::vector< std::vector<Room> > level;
        /* Structure of level array:
                   col0 col1 col2 ... col#
            row0:  0-0  0-1  0-2  ... 0-totalHorizontal
            row1:  1-0  ...
            row2:  2-0  ...
            ...    ...
            row#:  totalVertical-0  ...
        */
        Hero* hero;

        Game();
        ~Game();

        void checkRoom();
        void initLevel();
        void fillLevel();
    protected:
    private:
};

#endif // GAME_H
