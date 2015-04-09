#include "globals.h"

void initializeLevels(){

    level[10][2] = new Level(13,1);
    level[11][2] = new Level(7,0);
    level[9][2] = new Level(7,0);
    level[10][3] = new Level(1,0);
    level[10][2]->horizontalPosition = 10;
    level[10][2]->verticalPosition = 2;
    level[11][2]->horizontalPosition = 11;
    level[11][2]->verticalPosition = 2;
    level[9][2]->horizontalPosition = 9;
    level[9][2]->verticalPosition = 2;
    level[10][3]->horizontalPosition = 10;
    level[10][3]->verticalPosition = 3;

    currentHorizontalLevel = 10;
    currentVerticalLevel = 2;

    createPlatform(500,50,450,50,level[10][2]);
    createPlatform(100,25,200,150,level[10][2]);
    createPlatform(100,25,500,150,level[10][2]);
    createPlatform(100,15,600,180,level[10][2]);
    createPlatform(15,15,650,260,level[10][2]);
    createPlatform(25,15,690,290,level[10][2]);
    createPlatform(200,5,400,320,level[10][2]);
    createPlatform(200,5,300,400,level[10][2]);
    createPlatform(200,5,400,520,level[10][2]);
    createPlatform(200,20,750,420,level[10][2]);
    createPlatform(200,10,750,600,level[10][2]);
    createPlatform(200,5,550,650,level[10][2]);
    createPlatform(200,5,300,680,level[10][2]);

    createPlatform(500,50,500,50,level[11][2]);
    createPlatform(100,20,500,120,level[11][2]);
    createPlatform(100,20,400,200,level[11][2]);
    createPlatform(100,20,300,280,level[11][2]);
    createPlatform(100,20,200,360,level[11][2]);
    createPlatform(100,20,100,420,level[11][2]);
    createPlatform(100,20,100,620,level[11][2]);

    createPlatform(500,50,500,50,level[9][2]);
    createPlatform(50,20,500,120,level[9][2]);
    createPlatform(50,20,400,200,level[9][2]);
    createPlatform(50,20,300,280,level[9][2]);
    createPlatform(50,20,200,360,level[9][2]);
    createPlatform(50,20,100,420,level[9][2]);
    createPlatform(50,20,100,620,level[9][2]);

    createPlatform(200,5,200,5,level[10][3]);
    createBasicEnemy(150,200,150,250,level[10][2]);
}
