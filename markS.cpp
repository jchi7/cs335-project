//Author: Mark Stevens
//
//Description:
//
//This function handles all of our game input when playing the game in regular
//and Level Editor mode. It does not, however, handle input when in map mode or
//during credits. These are handled in game.cpp
//
// There are also some helper functions at the bottom of the file that are 
// used to clean up the code in check_game_input() function.
//
// The first function movablePlatformCollision() handles the pseudo snapping of
// platforms against each other. Essentially what this means is that when you
// move platforms in level editor mode you cannot move one platform on top of
// another. They will not overlap in any way. Instead the movable platform will
// slide along the outside of the stationary platform that it is trying to overlap


#include "structs.h"
#include "gameObject.h"
#include "hero.h"
#include "basicEnemy.h"
#include "platform.h"
#include "spike.h"
#include "room.h"
#include "ppm.h"
#include "game.h"
#include "collisions.h"
#include "fernandoV.h"
#include "markS.h"
#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 700

void movablePlatformCollision(GameObject * movablePlatform, GameObject * stationaryPlatform){

    // This function may be a bit hard to follow. The code seems to work fairly well. The trouble
    // is trying to figure out if the movable platform should go above/below the stationary platform
    // or to the left/right of the stationary platform.

    float movablePlatformLeft = movablePlatform->body.center[0] - movablePlatform->body.width;
    float movablePlatformRight = movablePlatform->body.center[0] + movablePlatform->body.width;
    float movablePlatformTop = movablePlatform->body.center[1] + movablePlatform->body.height;
    float movablePlatformBottom = movablePlatform->body.center[1] - movablePlatform->body.height;

    float stationaryPlatformLeft = stationaryPlatform->body.center[0] - stationaryPlatform->body.width;
    float stationaryPlatformRight = stationaryPlatform->body.center[0] + stationaryPlatform->body.width;
    float stationaryPlatformTop = stationaryPlatform->body.center[1] + stationaryPlatform->body.height;
    float stationaryPlatformBottom = stationaryPlatform->body.center[1] - stationaryPlatform->body.height;

    // Check if the the right side of the movable platform is less than 10 pixels to the right of the left
    // side of the stationary platform
    if (movablePlatformRight < stationaryPlatformLeft + 10){
        // check if the movable platform is not within 5 pixels of the top or bottom of the stationary 
        // one
        if ( !(movablePlatformTop > stationaryPlatformTop && 
                movablePlatformBottom > stationaryPlatformTop - 5)
                &&
                !(movablePlatformBottom < stationaryPlatformBottom &&
                 movablePlatformTop < stationaryPlatformBottom + 5))
        {
            movablePlatform->body.center[0] = stationaryPlatformLeft - movablePlatform->body.width;
            // return here so that the movable platform doesnt get moved on its y axis further down 
            // in this function
            return;
        }
    }
    // Check if the the left side of the movable platform is less than 10 pixels to the left of the right
    // side of the stationary platform
    if (movablePlatformLeft > stationaryPlatformRight - 10){
        // check if the movable platform is not within 5 pixels of the top or bottom of the stationary 
        // one
        if ( !(movablePlatformTop > stationaryPlatformTop && 
                movablePlatformBottom > stationaryPlatformTop - 5)
                &&
                !(movablePlatformBottom < stationaryPlatformBottom &&
                 movablePlatformTop < stationaryPlatformBottom + 5))
        {
            movablePlatform->body.center[0] = stationaryPlatformRight + movablePlatform->body.width;
            // return here so that the movable platform doesnt get moved on its y axis further down 
            // in this function
            return;
        }
    }

    // If we get to this point then the movable platform will either be placed above the stationary one
    // or below it based on these next two if statements

    if (movablePlatform->body.center[1] < stationaryPlatform->body.center[1]){
        movablePlatform->body.center[1] = stationaryPlatformBottom - movablePlatform->body.height;
        return;
    }
    if (movablePlatform->body.center[1] > stationaryPlatform->body.center[1]){
        movablePlatform->body.center[1] = stationaryPlatformTop + movablePlatform->body.height;
        return;
    }
}


void check_game_input(XEvent *e, Game *game)
{
    GameObject mouse;
    mouse.body.type = RECTANGLE;
    mouse.body.width = 1;
    mouse.body.height = 1;
    mouse.body.center[0] = e->xbutton.x;
    mouse.body.center[1] = WINDOW_HEIGHT - e->xbutton.y;

    if (e->type == KeyPress){
        //cout << e->xbutton.x << endl;
        int key = XLookupKeysym(&e->xkey,0);
        if (key == XK_m && !game->isPlatformMovable &&
                !game->isElevatorMovable &&
                !game->isPlatformResizable &&
                !game->isSpikeMovable &&
                !game->isSavePointMovable &&
                !game->isEnemyMovable){
            if (g_gamestate == MAP)
                g_gamestate = LEVEL_EDITOR;
            else if (g_gamestate == LEVEL_EDITOR)
                g_gamestate = MAP;
        }

        if (key == XK_Left){
            game->hero->leftPressed = 1;
        }
        if (key == XK_Right){
            game->hero->rightPressed = 1;
        }
        if (key == XK_Escape){
            g_gamestate = MAIN_MENU;
			stopGameMusic();
            playMenuMusic();
        }
        if (key == XK_e){
            if (game->hero->delay == 0) {
                game->heroShoots();
                game->hero->delay++;
            }
        }

        if ((key == XK_Up || key == XK_w || key == XK_space) && game->hero->jumpRelease == 0){
            if (game->hero->state == WALKING || game->hero->state == STANDING){
                game->hero->initialJump = 1;
            }
            if (game->hero->state == JUMPING && game->hero->jumpCount < 2){
                game->hero->secondJump = 1;
            }
        }
        if (game->state == LEVEL_EDITOR){
            if (key == XK_b){
                game->saveRooms();
            }
            if (!game->isPlatformMovable &&
                    !game->isPlatformResizable &&
                    !game->isElevatorMovable &&
                    !game->isElevatorResizable &&
                    !game->isSpikeMovable &&
                    !game->isSavePointMovable &&
                    !game->isEnemyMovable){
                if (key == XK_j){
                    game->moveRoomLeft();
                }
                if (key == XK_l){
                    game->moveRoomRight();
                }
                if (key == XK_k){
                    game->moveRoomDown();
                }
                if (key == XK_i){
                    game->moveRoomUp();
                }
            }
            if (key == XK_5){
                if (game->isHeroMovable)
                    game->isHeroMovable = false;
                else
                    game->isHeroMovable = true;
            }
            if (key == XK_Shift_L){
                if (game->isPlatformMovable)
                {
                    Room * curRoom = game->getRoomPtr();
                    curRoom->platforms[game->movablePlatformIndex]->tex_id++;
                    curRoom->platforms[game->movablePlatformIndex]->tex_id %= 5;
                     
                }
                if (currentlyEditable(game))
                {
                    editorAddPlatform(game, &mouse);
                }
            }
            if (key == XK_a){
                if (currentlyEditable(game))
                {
                    editorAddSavePoint(game, &mouse);
                }
            }
            if (key == XK_f){
                if (game->isEnemyMovable)
                {
                    nextEnemy(game, &mouse);
                }
                if (currentlyEditable(game))
                {
                    editorAddEnemy(game, &mouse);
                }
            }
            if (key == XK_g){
                if (game->isElevatorMovable)
                {
                    Room * curRoom = game->getRoomPtr();
                    curRoom->elevators[game->movableElevatorIndex]->tex_id++;
                    curRoom->elevators[game->movableElevatorIndex]->tex_id %= 5;
                     
                }
                if (currentlyEditable(game))
                {
                    editorAddElevator(game, &mouse);
                }
            }
            if (key == XK_h)
            {
                if (game->isElevatorMovable ||
                        game->isElevatorResizable)
                {
                    Elevator * curElevator = game->getWorkingElevatorPtr();
                    int nextSpeed = curElevator->getVertSpeed();
                    nextSpeed++;
                    curElevator->setVertSpeed(nextSpeed % 20);
                }
            }
            if (key == XK_s){
                if (game->isSpikeMovable){
                    GameObject * curSpike = game->getWorkingSpikePtr();
                    ShapeOrientation spikeFacing = curSpike->body.orientation;

                    if (curSpike->body.orientation == FACING_UP){
                        spikeFacing = FACING_LEFT;
                    }
                    else if (curSpike->body.orientation == FACING_LEFT){
                        spikeFacing = FACING_DOWN;
                    }
                    else if (curSpike->body.orientation == FACING_DOWN){
                        spikeFacing = FACING_RIGHT;
                    }
                    else if (curSpike->body.orientation == FACING_RIGHT){
                        spikeFacing = FACING_UP;
                    }
                    curSpike->body.orientation = spikeFacing;
                }
                if (currentlyEditable(game))
                {
                    editorAddSpike(game, &mouse);
                }
            }
            if (key == XK_x){
                if (game->isPlatformMovable){
                    game->isPlatformMovable = false;
                }
                if (game->isPlatformResizable){
                    game->isPlatformResizable = false;
                }
                if (game->isElevatorMovable){
                    game->isElevatorMovable = false;
                }
                if (game->isElevatorResizable){
                    game->isElevatorResizable = false;
                }
                if (game->isSpikeMovable){
                    game->isSpikeMovable = false;
                }
                if (game->isSavePointMovable){
                    game->isSavePointMovable = false;
                }
                if (game->isEnemyMovable){
                    game->isEnemyMovable = false;
                }
            }
            if (key == XK_z){
                if (currentlyEditable(game))
                {
                    Room * room = game->getRoomPtr();
                    if (!game->isPlatformResizable &&
                            !game->isElevatorResizable){
                        for (unsigned int k = 0; k < room->savePoints.size(); k++){
                            if (collisionRectRect(&mouse.body, &room->savePoints[k]->body)){
                                game->movableSavePointIndex = k;
                                game->isSavePointMovable = true;
                                break;
                            }
                        }
                    }
                    if (!game->isSavePointMovable &&
                            !game->isPlatformResizable &&
                            !game->isElevatorResizable){
                        vector<GameObject*> * spikesV = game->getSpikesVPtr();
                        for (unsigned int k = 0; k < spikesV->size(); k++){
                            if (collisionRectTri(&mouse.body, &spikesV->at(k)->body)){
                                game->movableSpikeIndex = k;
                                game->isSpikeMovable = true;
                                break;
                            }
                        }
                    }
                    if (!game->isSpikeMovable &&
                      !game->isSavePointMovable &&
                      !game->isPlatformResizable &&
                      !game->isElevatorResizable)
                    {
                        vector<GameObject*> * platformsV = game->getPlatformsVPtr();
                        for (unsigned int k = 0; k < platformsV->size(); k++){
                            if (collisionRectRect(&mouse.body,&platformsV->at(k)->body)){
                                game->movablePlatformIndex = k;
                                game->isPlatformMovable = true;
                                break;
                            }
                        }
                    }
                    if (!game->isSpikeMovable &&
                      !game->isSavePointMovable &&
                      !game->isPlatformMovable &&
                      !game->isPlatformResizable &&
                      !game->isElevatorResizable)
                    {
                        vector<GameObject*> * enemiesV = game->getEnemiesVPtr();
                        for (unsigned int k = 0; k < enemiesV->size(); k++){
                            if (collisionRectRect(&mouse.body,&enemiesV->at(k)->body)){
                                game->movableEnemyIndex = k;
                                game->isEnemyMovable = true;
                                break;
                            }
                        }
                    }
                    if (!game->isSpikeMovable &&
                      !game->isSavePointMovable &&
                      !game->isEnemyMovable &&
                      !game->isPlatformMovable &&
                      !game->isPlatformResizable &&
                      !game->isElevatorResizable)
                    {
                        vector<Elevator*> * elevatorsV = game->getElevatorsVPtr();
                        for (unsigned int k = 0; k < elevatorsV->size(); k++){
                            if (collisionRectRect(&mouse.body,&elevatorsV->at(k)->body)){
                                game->movableElevatorIndex = k;
                                game->isElevatorMovable = true;
                                break;
                            }
                        }
                    }
                }
            }
            if (key == XK_c && currentlyEditable(game))
            {
                vector<GameObject*> * platformsV = game->getPlatformsVPtr();
                for (unsigned int k = 0; k < platformsV->size(); k++){
                    if (collisionRectRect(&mouse.body,&platformsV->at(k)->body)){
                        game->resizablePlatformIndex = k;
                        game->isPlatformResizable = true;
                        game->resizablePlatformX = platformsV->at(k)->body.center[0];
                        game->resizablePlatformY = platformsV->at(k)->body.center[1];
                    }
                }
                if (!game->isPlatformResizable) {
                    vector<Elevator*> * elevatorsV = game->getElevatorsVPtr();
                    for (unsigned int k = 0; k < elevatorsV->size(); k++){
                        if (collisionRectRect(&mouse.body,&elevatorsV->at(k)->body)){
                            game->resizableElevatorIndex = k;
                            game->isElevatorResizable = true;
                        }
                    }
                }
            }
            if (key == XK_d && currentlyEditable(game))
            {
                Room * room = game->getRoomPtr();
                for (unsigned int L = 0; L < room->savePoints.size(); L++){
                    if (collisionRectRect(
                      &mouse.body,
                      &room->savePoints[L]->body))
                    {
                        editorRemoveSavePoint(game, L);
                        return;
                    }
                }
                vector<GameObject*> * spikesV = game->getSpikesVPtr();
                for (unsigned int k = 0; k < spikesV->size(); k++){
                    if (collisionRectTri(
                      &mouse.body,
                      &spikesV->at(k)->body))
                    {
                        editorRemoveSpike(game, k);
                        return;
                    }
                }
                vector<GameObject*> * platformsV = game->getPlatformsVPtr();
                for (unsigned int p = 0; p < platformsV->size(); p++){
                    if (collisionRectRect(
                      &mouse.body,
                      &platformsV->at(p)->body))
                    {
                        editorRemovePlatform(game, p);
                        return;
                    }
                }
                vector<GameObject*> * enemiesV = game->getEnemiesVPtr();
                for (unsigned int p = 0; p < enemiesV->size(); p++){
                    if (collisionRectRect(
                      &mouse.body,
                      &enemiesV->at(p)->body))
                    {
                        editorRemoveEnemy(game, p);
                        return;
                    }
                }
                vector<Elevator*> * elevatorsV = game->getElevatorsVPtr();
                for (unsigned int e = 0; e < elevatorsV->size(); e++){
                    if (collisionRectRect(
                      &mouse.body,
                      &elevatorsV->at(e)->body))
                    {
                        editorRemoveElevator(game, e);
                        return;
                    }
                }
            }
        }
    }
    if (e->type == KeyRelease){
        int key = XLookupKeysym(&e->xkey,0);
        if ( key == XK_Left){
            game->hero->leftPressed = 0;
        }
        if ( key == XK_Right){
            game->hero->rightPressed = 0;
        }
        if ( key == XK_w || key == XK_Up || key == XK_space){
            game->hero->jumpRelease = 4;
        }
    }
    if (game->isPlatformResizable){
        resizePlatform(game, &mouse);
    }
    else if (game->isElevatorResizable){
        resizeElevator(game, &mouse);
    }
}

void editorAddPlatform(Game * game, GameObject * mouse)
{
    Room * room = game->getRoomPtr();
    room->platforms.push_back(
      new Platform(
        game->platformTextureWidth,
        game->platformTextureHeight,
        mouse->body.center[0],
        mouse->body.center[1],
        0)); // !!!! should be texture's current id !!!!
    room->numPlatforms++;
    game->movablePlatformIndex = room->platforms.size() - 1;
    game->isPlatformMovable = true;
}

void editorAddEnemy(Game * game, GameObject * mouse)
{
    Room * room = game->getRoomPtr();
    room->enemies.push_back(
        new BasicEnemy(
            mouse->body.center[0],
            mouse->body.center[1]));
    room->numBasicEnemies++;
    game->movableEnemyIndex = room->enemies.size() - 1;
    game->isEnemyMovable = true;
}

void nextEnemy(Game *game, GameObject * mouse)
{
    Room * room = game->getRoomPtr();
    cout << "size before nextEnemy: " << room->enemies.size() << " ";
    ObjectType id = room->enemies[game->movableEnemyIndex]->id;
    delete room->enemies[game->movableEnemyIndex];
    switch (id){
        case ENEMY:
            room->enemies[game->movableEnemyIndex] = new ShooterEnemy(
                mouse->body.center[0], mouse->body.center[1]);
            break;
        case SHOOTERENEMY:
            room->enemies[game->movableEnemyIndex] = new BasicEnemy(
                mouse->body.center[0], mouse->body.center[1]);
            break;
        default:
            break;
    }
    cout << "size after nextEnemy: " << room->enemies.size() << endl;
}

void editorAddSpike(Game * game, GameObject * mouse)
{
    Room * room = game->getRoomPtr();
    Vec spike[3];
    spike[0][0] = mouse->body.center[0];
    spike[0][1] = mouse->body.center[1];
    spike[0][2] = 0;
    spike[1][0] = mouse->body.center[0] + 26;
    spike[1][1] = mouse->body.center[1];
    spike[1][2] = 0;
    spike[2][0] = mouse->body.center[0] + 13;
    // (sqrt(3) / 2 ) * width  = height for equilateral triangle
    spike[2][1] = mouse->body.center[1] + 22.981;
    spike[2][2] = 0;

    room->spikes.push_back(
      new Spike(spike,FACING_UP));
    room->numSpikes++;
    game->movableSpikeIndex = room->spikes.size() - 1;
    game->isSpikeMovable = true;
}

void editorAddSavePoint(Game * game, GameObject * mouse)
{
    Room * room = game->getRoomPtr();
    room->savePoints.push_back(
      new SavePoint(10,10,mouse->body.center[0],mouse->body.center[1]));
    room->numSavePoints++;
    game->isSavePointMovable = true;
    game->movableSavePointIndex = room->savePoints.size() - 1;
}

void editorAddElevator(Game *game, GameObject * mouse)
{
    Room * room = game->getRoomPtr();
    room->elevators.push_back(
      new Elevator(
        mouse->body.center[0],
        mouse->body.center[1],
        mouse->body.center[1] + (game->platformTextureHeight * 2),
        0, 1
    ));
    room->numElevators++;
    game->movableElevatorIndex = room->elevators.size() - 1;
    game->isElevatorMovable = true;
}

void resizePlatform(Game * game, GameObject * mouse)
{
    GameObject * platform = game->getWorkingPlatformPtr();
    int mouseX = (int)mouse->body.center[0];
    int mouseY = (int)mouse->body.center[1];
    int height = (( ( mouseY - game->resizablePlatformY) / ( platform->textureHeight)) * platform->textureHeight + platform->textureHeight);
    int width = (( ( mouseX - game->resizablePlatformX) / ( platform->textureWidth)) * platform->textureWidth + platform->textureWidth);

    if ( width <= 0){
        platform->body.width = platform->textureWidth;
    }
    else{
        platform->body.width = width;
        platform->horizontalTiles = platform->body.width / platform->textureWidth;
    }
    if ( height <= 0){
        platform->body.height = platform->textureHeight;
    }
    else{
        platform->body.height = height;
        platform->verticalTiles = platform->body.height / platform->textureHeight;
    }
}

void resizeElevator(Game *game, GameObject * mouse)
{
    Elevator * elevator = game->getWorkingElevatorPtr();
    elevator->setUpperLimit(mouse->body.center[1]);
}

void editorRemovePlatform(Game * game, int index)
{
    Room * room = game->getRoomPtr();
    delete room->platforms[index];
    room->platforms.erase(room->platforms.begin() + index);
    room->numPlatforms--;
}

void editorRemoveSpike(Game * game, int index)
{
    Room * room = game->getRoomPtr();
    delete room->spikes[index];
    room->spikes.erase(room->spikes.begin() + index);
    room->numSpikes--;
}

void editorRemoveSavePoint(Game * game, int index)
{
    Room * room = game->getRoomPtr();
    delete room->savePoints[index];
    room->savePoints.erase(room->savePoints.begin() + index);
    room->numSavePoints--;
}

void editorRemoveEnemy(Game * game, int index)
{
    Room * room = game->getRoomPtr();
    delete room->enemies[index];
    room->enemies.erase(room->enemies.begin() + index);
    room->numBasicEnemies--;
}

void editorRemoveElevator(Game *game, int index)
{
    Room * room = game->getRoomPtr();
    delete room->elevators[index];
    room->elevators.erase(room->elevators.begin() + index);
    room->numElevators--;
}
bool currentlyEditable(Game * game)
{
    if (!game->isPlatformMovable &&
            !game->isSpikeMovable &&
            !game->isEnemyMovable &&
            !game->isPlatformResizable &&
            !game->isElevatorMovable &&
            !game->isElevatorResizable &&
            !game->isSavePointMovable &&
            !game->isHeroMovable)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void check_death_input(XEvent *e,Game *game)
{
    if (e -> type ==KeyPress ) {
        int key = XLookupKeysym(&e->xkey,0);
        if (currentlyEditable(game)){
            if (key == XK_Escape){
                g_gamestate = MAIN_MENU;
            }
            if (key == XK_Return || key == XK_space) {
                game->respawnAtSavePoint();
                renderNum = 0;
            }
        }
        if (key == XK_x){
            if (game->isPlatformMovable){
                game->isPlatformMovable = false;
            }
            if (game->isPlatformResizable){
                game->isPlatformResizable = false;
            }
            if (game->isElevatorMovable){
                game->isElevatorMovable = false;
            }
            if (game->isElevatorResizable){
                game->isElevatorResizable = false;
            }
            if (game->isSpikeMovable){
                game->isSpikeMovable = false;
            }
            if (game->isSavePointMovable){
                game->isSavePointMovable = false;
            }
            if (game->isEnemyMovable){
                game->isEnemyMovable = false;
            }
        }

    }
}
