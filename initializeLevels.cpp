#include "initializeLevels.h"

Level*** initializeLevels()
{
    Level*** room = (Level***)malloc(20 * sizeof(Level**));
    room[10][2] = new Level(13,1);
    room[11][2] = new Level(7,0);
    room[9][2] = new Level(7,0);
    room[10][3] = new Level(1,0);
    room[10][2]->horizontalPosition = 10;
    room[10][2]->verticalPosition = 2;
    room[11][2]->horizontalPosition = 11;
    room[11][2]->verticalPosition = 2;
    room[9][2]->horizontalPosition = 9;
    room[9][2]->verticalPosition = 2;
    room[10][3]->horizontalPosition = 10;
    room[10][3]->verticalPosition = 3;

    room[10][2]->objects.push_back(new platform(500,50,450,50));
    room[10][2]->objects.push_back(new platform(100,25,200,150));
    room[10][2]->objects.push_back(new platform(100,25,500,150));
    room[10][2]->objects.push_back(new platform(100,15,600,180));
    room[10][2]->objects.push_back(new platform(15,15,650,260));

    return room;
}
