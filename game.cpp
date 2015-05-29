#include "game.h"
#include "fernandoV.h"
#include "fonts.h"
#include "markS.h"
using namespace std;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

Game::Game()
{
    this->leftPressed = 0;
    this->rightPressed = 0;
    this->shootPressed = 0;
    this->currentHorizontalLevel = 0;
    this->currentVerticalLevel = 0;
    this->savePointHorizontalRoom = 0;
    this->savePointVerticalRoom = 0;
    this->savePointIndex = 0;
    this->totalHorizontal = 20;
    this->totalVertical = 10;
    this->state = MAIN_MENU;
    
    this->isPlatformMovable = false;
    this->isPlatformResizable = false;
    this->isSpikeMovable = false;
    this->isSavePointMovable = false;
    this->isElevatorMovable = false;
    this->isElevatorResizable = false;
    this->isEnemyMovable = false;
    this->isHeroMovable = false;
    
    this->movablePlatformIndex = 0;
    this->movableSpikeIndex = 0;
    this->movableSavePointIndex = 0;
    this->movableElevatorIndex = 0;
    this->movableEnemyIndex = 0;

    this->resizablePlatformIndex = 0;    
    this->resizablePlatformX = 0;
    this->resizablePlatformY = 0;
    this->resizableElevatorIndex = 0;

    this->creditsTextPosition = 0;

    this->platformTextureHeight = 15;
    this->platformTextureWidth = 15;
    initLevel();
    fillLevel();
    initializeMap(totalHorizontal, totalVertical);
}

Game::~Game()
{
    delete hero;
    level.erase(level.begin(), level.end());

    for (int i = 0; i < totalVertical * totalHorizontal; i++)
    {
        delete mapGrid[i];
    }//dt r
}
void Game::initializeMap(int numHorizontal, int numVertical)
{
    float left = 200;
    //float right = 900;
    //float top = 650;
    float bottom = 350;
    float gridWidth = 700 / numHorizontal;
    float gridHeight = 300 / numVertical;
    Grid *currentGrid;
    for (int column = 0; column < numHorizontal; column++)
    {
        for (int row = 0; row < numVertical; row++)
        {
            mapGrid.push_back(new Grid());
            currentGrid = mapGrid[mapGrid.size()-1];
            currentGrid->r.left = left + (column * gridWidth);
            currentGrid->r.bot = bottom + (row * gridHeight);
            currentGrid->r.right = currentGrid->r.left + gridWidth;
            currentGrid->r.top = currentGrid->r.bot + gridHeight;
            currentGrid->r.width = gridWidth;
            currentGrid->r.height = gridHeight;
            currentGrid->over = false;
            currentGrid->click = false;
            currentGrid->color[0] = 50;
            currentGrid->color[1] = 100;
            currentGrid->color[2] = 100;
            currentGrid->hoverColor[0] = 100;
            currentGrid->hoverColor[1] = 100;
            currentGrid->hoverColor[2] = 250;
            currentGrid->currentCellColor[0] = 150;
            currentGrid->currentCellColor[1] = 200;
            currentGrid->currentCellColor[2] = 200;
            currentGrid->horizontalRoom = column;
            currentGrid->verticalRoom = row;
//            cout << currentGrid->r.left << " " << currentGrid->r.right << " " << currentGrid->r.bot << " " << currentGrid->r.top << endl;
        }
    }
}
void Game::checkMapInput(XEvent *e)
{
    int xClick;
    int yClick;
    if (e->type == ButtonPress)
    {
        if (e->xbutton.button == 1)
        {
            xClick = e->xbutton.x;
            yClick = WINDOW_HEIGHT - e->xbutton.y;
            for ( int row = 0; row < totalHorizontal * totalVertical; row++)
            {
                if (xClick > mapGrid[row]->r.left &&
                        xClick < mapGrid[row]->r.right &&
                        yClick > mapGrid[row]->r.bot &&
                        yClick < mapGrid[row]->r.top)
                {
                    cout << mapGrid[row]->horizontalRoom << endl;
                    currentHorizontalLevel = mapGrid[row]->horizontalRoom;
                    currentVerticalLevel = mapGrid[row]->verticalRoom;
                    g_gamestate = LEVEL_EDITOR;
                    return;
                }
            }
        }
    }
    if (e->type == KeyPress)
    {
        int key = XLookupKeysym(&e->xkey,0);
        if (key == XK_m){
            g_gamestate = LEVEL_EDITOR;
        }
        if (!isPlatformMovable &&
                !isPlatformResizable &&
                !isElevatorMovable &&
                !isElevatorResizable &&
                !isSpikeMovable &&
                !isSavePointMovable &&
                !isEnemyMovable)
        {
            if (key == XK_j){
                moveRoomLeft();
            }
            if (key == XK_l){
                moveRoomRight();
            }
            if (key == XK_k){
                moveRoomDown();
            }
            if (key == XK_i){
                moveRoomUp();
            }
        }
        if (key == XK_Escape){
            g_gamestate = MAIN_MENU;
        }
    }
}
void Game::renderMap(Display * dpy, Window * win)
{
    int root_x, root_y, win_x, win_y;
    int gridHoverHorizontal = -1;
    int gridHoverVertical = -1;
    unsigned int maskReturned;
    Window rootWin, childWin;
    XQueryPointer(dpy,*win,&rootWin,&childWin,&root_x,&root_y,&win_x,&win_y,&maskReturned);
    win_y = WINDOW_HEIGHT - win_y;
    int bottom = 350;
    int top = 650;
    int left = 200;
    int right = 900;
    int cellWidth = mapGrid[0]->r.width;
    int cellHeight = mapGrid[0]->r.height;
    glDisable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT);
    int row = 0;
    for (int row = 0; row < totalHorizontal * totalVertical; row++)
    {
        glPushMatrix();
        if(win_x < mapGrid[row]->r.right &&
                win_x > mapGrid[row]->r.left &&
                win_y < mapGrid[row]->r.top &&
                win_y > mapGrid[row]->r.bot)
        {
            gridHoverHorizontal = mapGrid[row]->horizontalRoom;
            gridHoverVertical = mapGrid[row]->verticalRoom;
            glColor3ub(mapGrid[row]->hoverColor[0], mapGrid[row]->hoverColor[1], mapGrid[row]->hoverColor[2]);
        }
        else if (currentHorizontalLevel == mapGrid[row]->horizontalRoom &&
                currentVerticalLevel == mapGrid[row]->verticalRoom)
        {
            glColor3ub(mapGrid[row]->currentCellColor[0], mapGrid[row]->currentCellColor[1], mapGrid[row]->currentCellColor[2]);
        }
        else
        {
            glColor3ub(mapGrid[row]->color[0],mapGrid[row]->color[1],mapGrid[row]->color[2]);
        }
        glBegin(GL_QUADS);
        glVertex2i(mapGrid[row]->r.left, mapGrid[row]->r.top);
        glVertex2i(mapGrid[row]->r.left, mapGrid[row]->r.bot);
        glVertex2i(mapGrid[row]->r.right, mapGrid[row]->r.bot);
        glVertex2i(mapGrid[row]->r.right, mapGrid[row]->r.top);
//        cout << mapGrid[row]->r.left << " " << mapGrid[row]->r.right << " " << mapGrid[row]->r.bot << " " << mapGrid[row]->r.top << endl;
        glEnd();
        glPopMatrix();
    }
    for (int row = 0; row <= totalVertical; row++)
    {
        glPushMatrix();
        glColor3ub(200,200,200);
        glBegin(GL_LINE_LOOP);
        glVertex2i(left, bottom + (row*cellHeight));
        glVertex2i(right,bottom + (row*cellHeight));
        glEnd();
        glPopMatrix();
    }
    for (int column = 0; column <= totalHorizontal; column++)
    {
        glPushMatrix();
        glColor3ub(200,200,200);
        glBegin(GL_LINE_LOOP);
        glVertex2i(left + (column*cellWidth), bottom);
        glVertex2i(left + (column*cellWidth), top);
        glEnd();
        glPopMatrix();
    }

    glPushMatrix();
    glColor3ub(mapGrid[row]->hoverColor[0], mapGrid[row]->hoverColor[1], mapGrid[row]->hoverColor[2]);
    glBegin(GL_QUADS);
    glVertex2i(70,600);
    glVertex2i(70,560);
    glVertex2i(110,560);
    glVertex2i(110,600);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(mapGrid[row]->currentCellColor[0], mapGrid[row]->currentCellColor[1], mapGrid[row]->currentCellColor[2]);
    glBegin(GL_QUADS);
    glVertex2i(70,450);
    glVertex2i(70,410);
    glVertex2i(110,410);
    glVertex2i(110,450);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(255,255,255);
    glBegin(GL_LINE_LOOP);
    glVertex2i(30,280);
    glVertex2i(970,280);
    glVertex2i(970,30);
    glVertex2i(30,30);
    glVertex2i(30,280);
    glEnd();
    glPopMatrix();
    
    glEnable(GL_TEXTURE_2D);
    Rect r;
    r.bot = 660;
    r.left = 450;
    r.center = 0;
    ggprint16(&r, 0, 0xffffffff, "Map and Key Binds");
    char str[10];
    for (int row = 0; row < totalVertical; row++)
    {
        str[0] = (row + 48);
        str[1] = 0;
        r.bot = bottom + 5 + row*cellHeight;
        r.left = left -20;
        r.center = 0;
        ggprint12(&r, 0, 0xff00ff00, str);
    }
    for (int row = 0; row < totalHorizontal; row++)
    {
        str[0] = (row/10) + 48;
        str[1] = (row % 10) + 48;
        str[2] = 0;
        r.bot = bottom -20;
        r.left = left + 5 + row*cellWidth;
        r.center = 0;
        ggprint12(&r, 0, 0xff00ff00, str);
    }
    //
    r.bot = 390;
    r.left = 30;
    r.center = 0;
    ggprint16(&r, 0, 0xffffffff, "Current Room");
    r.bot = 370;
    r.left = 50;
    r.center = 0;
    char coords[10];
    coords[0] = '(';
    coords[1] = (currentHorizontalLevel / 10) + 48;
    coords[2] = (currentHorizontalLevel % 10) + 48;
    coords[3] = ' ';
    coords[4] = ',';
    coords[5] = ' ';
    coords[6] = (currentVerticalLevel / 10) + 48;
    coords[7] = (currentVerticalLevel % 10) + 48;
    coords[8] = ')';
    coords[9] = 0;
    ggprint16(&r, 0, 0xffffffff, coords);
    r.bot = 540;
    r.left = 30;
    r.center = 0;
    ggprint16(&r, 0, 0xffffffff, "Mouse Hover");
    r.bot = 520;
    r.left = 50;
    r.center = 0;
    coords[1] = (gridHoverHorizontal / 10) + 48;
    coords[2] = (gridHoverHorizontal % 10) + 48;
    coords[6] = (gridHoverVertical / 10) + 48;
    coords[7] = (gridHoverVertical % 10) + 48;
    ggprint16(&r, 0, 0xffffffff, coords);

    r.bot = 290;
    r.left = 420;
    r.center = 0;
    ggprint16(&r,0,0xffffffff, "Level Editor KeyBinds");

    r.bot = 260;
    r.left = 150;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "Left Shift: Create/Switch Texture Platform");

    r.bot = 230;
    r.left = 150;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "A key: Create Save Point");

    r.bot = 200;
    r.left = 150;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "S key: Create/Rotate Spike");

    r.bot = 170;
    r.left = 150;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "G key: Create Elevator");

    r.bot = 140;
    r.left = 150;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "F key: Create/Toggle Enemies");

    r.bot = 110;
    r.left = 150;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "Z key: Grab And Move Objects");

    r.bot = 80;
    r.left = 150;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "X Key: Paste Object to Screen/Stop Resizing Object");

    r.bot = 50;
    r.left = 150;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "C key: Resize Platform");

    r.bot = 260;
    r.left = 550;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "D key: Delete Object");

    r.bot = 230;
    r.left = 550;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "B key: Save Changes to Current Room");

    r.bot = 200;
    r.left = 550;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "M key: Toggle This Map screen");

    r.bot = 170;
    r.left = 550;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "I key: Move Up One Room on the Grid");

    r.bot = 140;
    r.left = 550;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "J key: Move Left One Room on The Grid");

    r.bot = 110;
    r.left = 550;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "K key: Move Down One Room on The Grid");

    r.bot = 80;
    r.left = 550;
    r.center = 0;
    ggprint13(&r,0,0xffffffff, "L key: Move Right One Room on The Grid");

    /*    ggprint06(&r, 20, 0x00ff0000, "ggprint06");
    ggprint07(&r, 20, 0x00ffff00, "ggprint07");
    ggprint08(&r, 20, 0x00ffff00, "ggprint08");
    ggprint10(&r, 20, 0x00ffff00, "ggprint10");
    ggprint12(&r, 20, 0x00ffff00, "ggprint12");
    ggprint8b(&r, 20, 0x00ffff00, "ggprint8b");
    ggprint13(&r, 20, 0x00ff00ff, "ggprint13");
    ggprint16(&r, 20, 0x00ff0000, "ggprint16");*/
}

void Game::setSavePoint(int index)
{
    this->savePointHorizontalRoom = this->currentHorizontalLevel;
    this->savePointVerticalRoom = this->currentVerticalLevel;
    this->savePointIndex = index;
}

void Game::respawnAtSavePoint()
{
    Room * currentRoom;

    this->hero->state = JUMPING;
    this->hero->leftPressed = 0;
    this->hero->rightPressed = 0;

    this->currentHorizontalLevel = this->savePointHorizontalRoom;
    this->currentVerticalLevel = this->savePointVerticalRoom;
    currentRoom = getRoomPtr();
    this->hero->body.center[0] = currentRoom->savePoints[this->savePointIndex]->body.center[0];
    this->hero->body.center[1] = currentRoom->savePoints[this->savePointIndex]->body.center[1];
}
void Game::checkCreditsInput(XEvent *e)
{
    if (e->type == KeyPress)
    {
        int key = XLookupKeysym(&e->xkey,0);
        if ( key == XK_Escape)
        {
            g_gamestate = MAIN_MENU;
        }
    }
}

void Game::playCredits()
{
    glDisable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glColor3ub(255,255,255);
    glBegin(GL_LINE_LOOP);
    glVertex2i(400,195 + creditsTextPosition/4);
    glVertex2i(600,195 + creditsTextPosition/4);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3ub(255,255,255);
    glBegin(GL_LINE_LOOP);
    glVertex2i(410,-105 + creditsTextPosition/4);
    glVertex2i(590,-105 + creditsTextPosition/4);
    glEnd();
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);

    Rect creditsPosition[10];
    char * creditsText[10];
    char firstLine[50] = "Congratulations You Beat The Game!!\0";
    char secondLine[30] = "Development Team:\0";
    char thirdLine[30] = "Fernando Villarreal\0";
    char fourthLine[30] = "Jasjot \"Sunny\" Sumal\0";
    char fifthLine[30] = "Jason Chi\0";
    char sixthLine[30] = "Mark Stevens\0";
    char seventhLine[30] = "Moises Ayala\0";
    char eighthLine[30] = "Game Testers:\0";
    char ninthLine[30] = "Polo Melendez\0";
    char tenthLine[30] = "THANK YOU FOR PLAYING\0";

    creditsText[0] = firstLine;
    creditsText[1] = secondLine;
    creditsText[2] = thirdLine;
    creditsText[3] = fourthLine;
    creditsText[4] = fifthLine;
    creditsText[5] = sixthLine;
    creditsText[6] = seventhLine;
    creditsText[7] = eighthLine;
    creditsText[8] = ninthLine;
    creditsText[9] = tenthLine;

    creditsPosition[0].left = 350;
    creditsPosition[0].bot = 300 + creditsTextPosition/4;
    creditsPosition[0].center = 0;

    creditsPosition[1].left = 420;
    creditsPosition[1].bot = 200 + creditsTextPosition/4;
    creditsPosition[1].center = 0;

    creditsPosition[2].left = 440;
    creditsPosition[2].bot = 160 + creditsTextPosition/4;
    creditsPosition[2].center = 0;

    creditsPosition[3].left = 440;
    creditsPosition[3].bot = 120 + creditsTextPosition/4;
    creditsPosition[3].center = 0;

    creditsPosition[4].left = 440;
    creditsPosition[4].bot = 80 + creditsTextPosition/4;
    creditsPosition[4].center = 0;

    creditsPosition[5].left = 440;
    creditsPosition[5].bot = 40 + creditsTextPosition/4;
    creditsPosition[5].center = 0;

    creditsPosition[6].left = 440;
    creditsPosition[6].bot = 0 + creditsTextPosition/4;
    creditsPosition[6].center = 0;

    creditsPosition[7].left = 430;
    creditsPosition[7].bot = -100 + creditsTextPosition/4;
    creditsPosition[7].center = 0;

    creditsPosition[8].left = 440;
    creditsPosition[8].bot = -150 + creditsTextPosition/4;
    creditsPosition[8].center = 0;
    creditsPosition[9].left = 370;
    if (creditsTextPosition < 2400)
        creditsPosition[9].bot = -250 + creditsTextPosition/4;
    else
        creditsPosition[9].bot = 350;
    creditsPosition[9].center = 0;

    for (int i = 0; i < 10; i++){
        if ( i < 2 || i == 7 || i == 9)
            ggprint16(&creditsPosition[i], 0, 0xffffffff, creditsText[i]);
        else
            ggprint13(&creditsPosition[i], 0, 0x00ff0000, creditsText[i]);

    }
    creditsTextPosition++;

}

void Game::checkRoom()
{
    Room *oldRoom;
    Room *newRoom;
    if (hero->body.center[0] > WINDOW_WIDTH){
        if (isPlatformMovable || isSpikeMovable || isSavePointMovable || isEnemyMovable || isPlatformResizable || isElevatorMovable || isElevatorResizable){
            hero->body.center[0] = WINDOW_WIDTH - hero->body.width;
        }
        else{
            if (currentHorizontalLevel > totalHorizontal - 2)
            {
                hero->body.center[0] = 500;
                hero->body.center[1] = 350;
            }
            else
            {
                oldRoom = getRoomPtr();
                oldRoom->clearBullets();
                currentHorizontalLevel++;
                hero->body.center[0] = 0 + hero->body.width;
                newRoom = getRoomPtr();
                newRoom->respawn();
            }
        }
        // DEBUG:
    //    cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[0] < 0){
        if (isPlatformMovable || isSpikeMovable || isSavePointMovable || isEnemyMovable || isPlatformResizable || isElevatorMovable || isElevatorResizable){
            hero->body.center[0] = 0 + hero->body.width;
        }
        else{
            if (currentHorizontalLevel <= 0)
            {
                hero->body.center[0] = 500;
                hero->body.center[1] = 350;
            }
            else
            {
                oldRoom = getRoomPtr();
                oldRoom->clearBullets();
                currentHorizontalLevel--;
                hero->body.center[0] = WINDOW_WIDTH - hero->body.width;
                newRoom = getRoomPtr();
                newRoom->respawn();
            }
        }
        // DEBUG:
      //  cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[1] > WINDOW_HEIGHT){
        if (isPlatformMovable || isSpikeMovable || isSavePointMovable || isEnemyMovable || isPlatformResizable || isElevatorMovable || isElevatorResizable){
            hero->body.center[1] = WINDOW_HEIGHT - hero->body.height;
        }
        else {
            if (currentVerticalLevel > totalVertical - 2)
            {
                hero->body.center[0] = 500;
                hero->body.center[1] = 350;
            }
            else
            {
                oldRoom = getRoomPtr();
                oldRoom->clearBullets();
                currentVerticalLevel++;
                hero->body.center[1] = 0 + hero->body.height;
                newRoom = getRoomPtr();
                newRoom->respawn();
            }
        }
        // DEBUG:
     //   cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (hero->body.center[1] < 0){
        if (isPlatformMovable || isSpikeMovable || isSavePointMovable || isEnemyMovable || isPlatformResizable || isElevatorMovable || isElevatorResizable){
            hero->body.center[1] = 0 +  hero->body.height;
        }
        else {
            if (currentVerticalLevel <= 0)
            {
                hero->body.center[0] = 500;
                hero->body.center[1] = 350;
            }
            else
            {
                oldRoom = getRoomPtr();
                oldRoom->clearBullets();
                currentVerticalLevel--;
                hero->body.center[1] = WINDOW_HEIGHT - hero->body.height;
                newRoom = getRoomPtr();
                newRoom->respawn();
            }
        }
        // DEBUG:
     //   cout << "room: " << currentVerticalLevel << "," << currentHorizontalLevel << endl;
    }
    if (currentHorizontalLevel == 19 && currentVerticalLevel == 9)
        g_gamestate = CREDITS;

}

Room * Game::getRoomPtr()
{
    return &level[currentVerticalLevel][currentHorizontalLevel];
}

GameObject * Game::getWorkingPlatformPtr()
{
    if (!this->isPlatformMovable && !this->isPlatformResizable) {
        cout << "ERROR getWorkingPlatformPtr: platform not resizeable or movable\n";
        return NULL;
    }
    Room * room = this->getRoomPtr();
    if (this->isPlatformMovable) {
        return room->platforms[this->movablePlatformIndex];
    }
    return room->platforms[this->resizablePlatformIndex];
}

GameObject * Game::getWorkingSpikePtr()
{
    if (!this->isSpikeMovable) {
        cout << "ERROR getWorkingSpikePtr: spike not movable\n";
        return NULL;
    }
    Room * room = this->getRoomPtr();
    return room->spikes[this->movableSpikeIndex];
}

Elevator * Game::getWorkingElevatorPtr()
{
    if (!this->isElevatorMovable && !this->isElevatorResizable) {
        cout << "ERROR getWorkingElevatorPtr: elevator not resizable or movable\n";
        return NULL;
    }
    Room * room = this->getRoomPtr();
    if (this->isElevatorMovable) {
        return room->elevators[this->movableElevatorIndex];
    }
    return room->elevators[this->resizableElevatorIndex];
}

vector<GameObject*> * Game::getPlatformsVPtr()
{
    Room * room = this->getRoomPtr();
    return &(room->platforms);
}

vector<GameObject*> * Game::getSpikesVPtr()
{
    Room * room = this->getRoomPtr();
    return &(room->spikes);
}

vector<Elevator*> * Game::getElevatorsVPtr()
{
    Room * room = this->getRoomPtr();
    return &(room->elevators);
}


vector<GameObject*> * Game::getEnemiesVPtr()
{
    Room * room = this->getRoomPtr();
    return &(room->enemies);
}

void Game::moveRoomLeft()
{
    if (currentHorizontalLevel > 0)
        currentHorizontalLevel--;
}

void Game::moveRoomRight()
{
    if (currentHorizontalLevel < totalHorizontal-1)
        currentHorizontalLevel++;
}

void Game::moveRoomUp()
{
    if (currentVerticalLevel < totalVertical - 1)
        currentVerticalLevel++;
}

void Game::moveRoomDown()
{
    if (currentVerticalLevel > 0 )
        currentVerticalLevel--;
}

void Game::initLevel()
{
    std::vector<Room> tempRow;
    for (int vertical = 0; vertical < totalVertical; vertical++) {
        for (int horizontal = 0; horizontal < totalHorizontal; horizontal++) {
            tempRow.push_back(Room());
        }
        level.push_back(tempRow);
        tempRow.clear();
    }
}

void Game::fillLevel()
{
    string line, objType;
    string filename = "Rooms/room";
    ifstream file;
    const int pathsize = filename.length();
    char roomNum[] = "0000";
    ShapeOrientation orientation;

    // Room file format:
    // file name: roomCCRR.txt, RR = row number, CC = col number
    // lines:  (str)type,<data values for type>

    filename.append(roomNum);
    filename.append(".txt");
    for (int vert = 0; vert < totalVertical; vert++) {
        for (int horz = 0; horz < totalHorizontal; horz++) {
            // remove previous room number
            filename.erase(pathsize,4);

            // increment room numbers
            roomNum[0] = (char)((horz/10) + 48);
            roomNum[1] = (char)((horz%10) + 48);
            roomNum[2] = (char)((vert/10) + 48);
            roomNum[3] = (char)((vert%10) + 48);

            // insert new room number
            filename.insert(pathsize,roomNum);

            file.open(filename.c_str());
            if (!file.is_open()) {
                // DEBUG:
                cout << "Error: Could not open input file '" << filename << "'\n";
                continue;
            }
            else {
                // DEBUG:
                //cout << "Reading: " << filename << endl;
            }

            while (getline(file, line)) {

                if (!file.good()) {
                    break;
                }

                stringstream iss(line);

                string val;
                getline (iss, val, ',');
                stringstream typeConv(val);
                objType = typeConv.str();
                int convValTex;

                // read a single platform
                if (objType == "GROUND") {
                    float convVal[4];
                    for (int col = 0; col < 4; ++col) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        converter >> convVal[col];
                    }
                    getline(iss, val, ',');
                    stringstream converter(val);
                    converter >> convValTex;
                    // create platform
                    level[vert][horz].platforms.push_back(new Platform(convVal[0], convVal[1], convVal[2], convVal[3], convValTex));
                    level[vert][horz].numPlatforms++;
                    // DEBUG:
//                    cout << "Created platform in " << "[" << vert << "][" << horz <<"]\n";
                }
                // read a single savePoint
                else if (objType == "SAVEPOINT") {
                    float convVal[4];
                    for (int col = 0; col < 4; ++col) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        converter >> convVal[col];
                    }

                    // create savePoint
                    level[vert][horz].savePoints.push_back(new SavePoint(convVal[0], convVal[1], convVal[2], convVal[3]));
                    level[vert][horz].numSavePoints++;
                }
                // read a single spike
                else if (objType == "SPIKE") {
                    Vec spikePts[3];
                    int spikeFacing;
                    for (int col = 0; col < 6; ++col) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        // DEBUG:
                        //cout << converter.str() << endl;
                        converter >> spikePts[col/2][col%2];
                    }
                    for (int i = 0; i < 3; ++i) {
                        spikePts[i][2] = 0;
                    }
                    getline(iss, val, ',');
                    stringstream converter(val);
                    converter >> spikeFacing;
                    switch (spikeFacing) {
                        case 0:
                            orientation = FACING_UP;
                            break;
                        case 1:
                            orientation = FACING_LEFT;
                            break;
                        case 2:
                            orientation = FACING_DOWN;
                            break;
                        case 3:
                            orientation = FACING_RIGHT;
                            break;
                        case 4:
                        default:
                            cout << "Error: could not read spike orientation\n";
                    }
                    level[vert][horz].spikes.push_back(new Spike(spikePts,orientation));
                    level[vert][horz].numSpikes++;
                    // DEBUG:
//                    cout << "Created spike in " << "[" << vert << "][" << horz <<"]: " << vecPrint(spikePts[0]) << ", " << vecPrint(spikePts[1]) << ", " << vecPrint(spikePts[2]) << ", " << orientation << endl;
                }
                else if (objType == "ENEMY") {
                    float convVal[2];
                    for (int col = 0; col < 2; col++) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        converter >> convVal[col];
                    }
                    // create Enemy
                    level[vert][horz].enemies.push_back(new BasicEnemy(convVal[0], convVal[1]));
                    level[vert][horz].numBasicEnemies++;
                    level[vert][horz].spawnPoints.push_back(new SpawnPoint(convVal[0], convVal[1], ENEMY));
                }
                else if (objType == "SHOOTER") {
                    float convVal[2];
                    for (int col = 0; col < 2; col++) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        converter >> convVal[col];
                    }
                    // create Shooter Enemy
                    level[vert][horz].enemies.push_back(new ShooterEnemy(convVal[0], convVal[1]));
                    level[vert][horz].numBasicEnemies++;
                    level[vert][horz].spawnPoints.push_back(new SpawnPoint(convVal[0], convVal[1], SHOOTERENEMY));
                }
                else if (objType == "ELEVATOR") {
                    float convVal[4];
                    for (int col = 0; col < 3; col++) {
                        getline(iss, val, ',');
                        stringstream converter(val);
                        converter >> convVal[col];
                    }
                    getline(iss, val, ',');
                    stringstream converter(val);
                    converter >> convValTex;
                    getline(iss, val, ',');
                    stringstream converter2(val);
                    converter2 >> convVal[3];

                    // create Elevator
                    level[vert][horz].elevators.push_back(new Elevator(convVal[0], convVal[1], convVal[2], convValTex, convVal[3]));
                    level[vert][horz].numElevators++;
                }
            }
            file.close();
        }
    }
    // DEBUG:
//    cout << level[1][3].savePoints[0]->body.width << endl;
}

void Game::saveRooms()
{
    string line, roomType;
    string filename = "Rooms/room";
    ofstream file;

    const int pathsize = filename.length();
    char roomNum[] = "0000";

    // Room file format:
    // file name: roomCCRR.txt, RR = row number, CC = col number
    // line:  (int)width,(int)height,(int)center-x,(int)center-y,(str)type  

    filename.append(roomNum);
    filename.append(".txt");
    int vert = currentVerticalLevel;
    int horz = currentHorizontalLevel;
    // remove previous room number
    filename.erase(pathsize,4);

    // create actual room number
    roomNum[0] = (char)((horz/10) + 48);
    roomNum[1] = (char)((horz%10) + 48);
    roomNum[2] = (char)((vert/10) + 48);
    roomNum[3] = (char)((vert%10) + 48);

    // insert new room number
    filename.insert(pathsize,roomNum);

    file.open(filename.c_str(),std::ofstream::out);
    if (!file.is_open()) {
        cout << "Error: Could not open input file '" << filename << "'\n";
        return;
    }
    else {
        cout << "Writing: " << filename << endl;
    }

    for (unsigned int i = 0; i < level[vert][horz].platforms.size(); ++i) {
        writePlatform(level[vert][horz].platforms[i], file);
    }
    for (unsigned int i = 0; i < level[vert][horz].savePoints.size(); ++i) {
        writeSavePoint(level[vert][horz].savePoints[i], file);
    }
    for (unsigned int i = 0; i < level[vert][horz].spikes.size(); ++i) {
        writeSpike(level[vert][horz].spikes[i], file);
    }
    for (unsigned int i = 0; i < level[vert][horz].elevators.size(); ++i) {
        writeElevator(level[vert][horz].elevators[i], file);
    }
    for (unsigned int i = 0; i < level[vert][horz].enemies.size(); ++i) {
        writeEnemy(level[vert][horz].enemies[i], file);
    }
    file.close();
    return;
}

void Game::writePlatform(GameObject * platform, ofstream & outf)
{
    outf << "GROUND,"
      << platform->body.width << ","
      << platform->body.height << ","
      << platform->body.center[0] << ","
      << platform->body.center[1] << ","
      << platform->tex_id << "\n";
}

void Game::writeSavePoint(GameObject * savept, ofstream & outf)
{
    outf << "SAVEPOINT,"
      << savept->body.width << ","
      << savept->body.height << ","
      << savept->body.center[0] << ","
      << savept->body.center[1] << "\n";
}

void Game::writeEnemy(GameObject * enemy, ofstream & outf)
{
    switch (enemy->id)
    {
        case ENEMY:
            outf << "ENEMY,"
                << enemy->body.center[0] << ","
                << enemy->body.center[1] << "\n";
            break;
        case SHOOTERENEMY:
            outf << "SHOOTER,"
                << enemy->body.center[0] << ","
                << enemy->body.center[1] << "\n";
            break;
        default:
            break;
    }
}

void Game::writeSpike(GameObject * spike, ofstream & outf)
{
    int spikeFacing = 0;
    switch (spike->body.orientation) {
        case FACING_UP:
            spikeFacing = 0;
            break;
        case FACING_LEFT:
            spikeFacing = 1;
            break;
        case FACING_DOWN:
            spikeFacing = 2;
            break;
        case FACING_RIGHT:
            spikeFacing = 3;
            break;
        case STOP:
        default:
            break;
    }
    outf << "SPIKE,"
      << spike->body.corners[0][0] << ","
      << spike->body.corners[0][1] << ","
      << spike->body.corners[1][0] << ","
      << spike->body.corners[1][1] << ","
      << spike->body.corners[2][0] << ","
      << spike->body.corners[2][1] << ","
      << spikeFacing << "\n";
}

void Game::writeElevator(Elevator * elev, ofstream & outf)
{
    outf << "ELEVATOR,"
      << elev->body.center[0] << ","
      << elev->getLowerLimit() << ","
      << elev->getUpperLimit() << ","
      << elev->tex_id << ","
      << elev->getVertSpeed() << "\n";
}

void Game::heroShoots()
{
	//play laser
	playLaser();
    Room* current_level = getRoomPtr();
    switch(hero->body.orientation) {
        case FACING_RIGHT:
            current_level->bullet.push_back(new BasicBullet(4, 0, hero->body.center[0] + hero->body.width + 2, hero->body.center[1], HERO));
            current_level->numBullet++;
            break;
        case FACING_LEFT:
            current_level->bullet.push_back(new BasicBullet(-4, 0, hero->body.center[0] - hero->body.width - 2, hero->body.center[1], HERO));
            current_level->numBullet++;
            break;
        default:
            break;
    }
}

