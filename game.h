#ifndef GAME_H
#define GAME_H

#include "gameObject.h"
#include "room.h"
#include "platform.h"
#include "spike.h"
#include "hero.h"
#include "savePoint.h"

class Game
{
    public:
        int leftPressed;
        int rightPressed;
        int shootPressed;
        GameState state;
        bool isPlatformMovable;
        bool isPlatformResizable;
        bool isSpikeMovable;
        bool isSavePointMovable;
        int movablePlatformIndex;
        int movableSpikeIndex;
        int movableSavePointIndex;
        int resizablePlatformIndex;
        int resizablePlatformX;
        int resizablePlatformY;
        int platformTextureWidth;
        int platformTextureHeight;

        int savePointHorizontalRoom;
        int savePointVerticalRoom;
        int savePointIndex;

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

        void respawnAtSavePoint();
        void setSavePoint(int index);
        void checkRoom();
        Room * getRoomPtr();
        void moveRoomLeft();
        void moveRoomRight();
        void moveRoomUp();
        void moveRoomDown();
        void initLevel();
        void fillLevel();
        void saveRooms();
        void resizePlatform(GameObject * mouse);
    protected:
    private:
};

#endif // GAME_H
