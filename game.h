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
        int totalHorizontal;  //number of columns (level width)
        int totalVertical;  // number of rows (level height)

        std::vector< std::vector<Room> > level;
        /* Structure of level array:
                   col0 col1 col2 ... col#
            row#:  totalVertical-0  ...
            ...    ...
            row2:  2-0  ...
            row1:  1-0  ...
            row0:  0-0  0-1  0-2  ... 0-totalHorizontal
        */
        Hero* hero;

        Game();
        ~Game();

        void checkRoom();
        Room * getRoomPtr();
        void moveRoomLeft();
        void moveRoomRight();
        void moveRoomUp();
        void moveRoomDown();
        void initLevel();
        void fillLevel();
    protected:
    private:
};

#endif // GAME_H
