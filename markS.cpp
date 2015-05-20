//Mark Stevens
//This function handles game input.


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
#include <chrono>
#include <GL/glx.h>
#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 700

void check_game_input(XEvent *e, Game *game)
{
    GameObject mouse;
    mouse.body.type = RECTANGLE;
    mouse.body.width = 1;
    mouse.body.height = 1;
    if (e->type == KeyPress){
        //cout << e->xbutton.x << endl;
        int key = XLookupKeysym(&e->xkey,0);
        if (key == XK_Left){
            game->hero->leftPressed = 1;
        }
        if (key == XK_Right){
            game->hero->rightPressed = 1;
        }
        if (key == XK_Escape){
            g_gamestate = MAIN_MENU;
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
            if (key == XK_5){
                game->hero->body.center[0] = e->xbutton.x;
                game->hero->body.center[1] = WINDOW_HEIGHT - e->xbutton.y;
            }
            if (key == XK_Shift_L){
                if (!game->isPlatformMovable && !game->isPlatformResizable && !game->isSpikeMovable){
                    Room * room = game->getRoomPtr();
                    room->platforms.push_back(new Platform(game->platformTextureWidth,game->platformTextureHeight,e->xbutton.x, WINDOW_HEIGHT - e->xbutton.y));
                    room->numPlatforms++;
                    game->isPlatformMovable = true;
                    game->movablePlatformIndex = room->platforms.size() - 1;
                }
            }
            if (key == XK_a){
                if (!game->isPlatformMovable && !game->isPlatformResizable && !game->isSpikeMovable){
                    Room * room = game->getRoomPtr();
                    room->savePoints.push_back(new SavePoint(10,10,e->xbutton.x, WINDOW_HEIGHT - e->xbutton.y));
                    room->numSavePoints++;
                    game->isSavePointMovable = true;
                    game->movableSavePointIndex = room->savePoints.size() - 1;
                }
            }
            if (key == XK_s){
                Room * room = game->getRoomPtr();
                Vec spike[3];
                if (game->isSpikeMovable){
                    if (room->spikes[game->movableSpikeIndex]->body.orientation == FACING_UP){
                        room->spikes[game->movableSpikeIndex]->body.orientation = FACING_LEFT;
                    }
                    else if (room->spikes[game->movableSpikeIndex]->body.orientation == FACING_LEFT){
                        room->spikes[game->movableSpikeIndex]->body.orientation = FACING_DOWN;
                    }
                    else if (room->spikes[game->movableSpikeIndex]->body.orientation == FACING_DOWN){
                        room->spikes[game->movableSpikeIndex]->body.orientation = FACING_RIGHT;
                    }
                    else if (room->spikes[game->movableSpikeIndex]->body.orientation == FACING_RIGHT){
                        room->spikes[game->movableSpikeIndex]->body.orientation = FACING_UP;
                    }
                }
                if (!game->isSpikeMovable && !game->isPlatformMovable && !game->isPlatformMovable){
                    spike[0][0] = e->xbutton.x;
                    spike[0][1] = WINDOW_HEIGHT - e->xbutton.y;
                    spike[0][2] = 0;
                    spike[1][0] = e->xbutton.x + 30;
                    spike[1][1] = WINDOW_HEIGHT - e->xbutton.y;
                    spike[1][2] = 0;
                    spike[2][0] = e->xbutton.x + 15;
                    // (sqrt(3) / 2 ) * width  = height for equilateral triangle
                    spike[2][1] = WINDOW_HEIGHT - e->xbutton.y + 25.981;
                    spike[2][2] = 0;
                    room->spikes.push_back(new Spike(spike,FACING_UP));
                    room->numSpikes++;
                    game->isSpikeMovable = true;
                    game->movableSpikeIndex = room->spikes.size() - 1;
                }
            }
            if (key == XK_x){
                if (game->isPlatformMovable){
                    game->isPlatformMovable = false;
                }
                if (game->isSpikeMovable){
                    game->isSpikeMovable = false;
                }
                if (game->isPlatformResizable){
                    game->isPlatformResizable = false;
                }
                if (game->isSavePointMovable){
                    game->isSavePointMovable = false;
                }
            }
            if (key == XK_z){
                if (!game->isPlatformMovable && !game->isPlatformResizable && !game->isSpikeMovable && !game->isSavePointMovable){
                    Room * room = game->getRoomPtr();
                    mouse.body.center[0] = e->xbutton.x;
                    mouse.body.center[1] = WINDOW_HEIGHT - e->xbutton.y;
                    for (unsigned int k = 0; k < room->savePoints.size(); k++){
                        if (collisionRectRect(&mouse.body, &room->savePoints[k]->body)){
                            game->movableSavePointIndex = k;
                            game->isSavePointMovable = true;
                            break;
                        }
                    }
                    if (!game->isSavePointMovable){
                        for (unsigned int k = 0; k < room->spikes.size(); k++){
                            if (collisionRectTri(&mouse.body, &room->spikes[k]->body)){
                                game->movableSpikeIndex = k;
                                game->isSpikeMovable = true;
                                break;
                            }
                        }
                    }
                    if (!game->isSpikeMovable && !game->isSavePointMovable){
                        for (unsigned int k = 0; k < room->platforms.size(); k++){
                            if (collisionRectRect(&mouse.body,&room->platforms[k]->body)){
                                game->movablePlatformIndex = k;
                                game->isPlatformMovable = true;
                                break;
                            }
                        }
                    }
                }
            }

            if (key == XK_c && game->isPlatformMovable == false && game->isPlatformMovable == false){
                Room * room = game->getRoomPtr();
                mouse.body.center[0] = e->xbutton.x;
                mouse.body.center[1] = WINDOW_HEIGHT - e->xbutton.y;
                for (unsigned int k = 0; k < room->platforms.size(); k++){
                    if (collisionRectRect(&mouse.body,&room->platforms[k]->body)){
                        game->resizablePlatformIndex = k;
                        game->isPlatformResizable = true;
                        game->resizablePlatformX = room->platforms[k]->body.center[0];
                        game->resizablePlatformY = room->platforms[k]->body.center[1];
                    }
                }
            }
            if (key == XK_d && !game->isPlatformMovable && !game->isPlatformResizable && !game->isSpikeMovable && !game->isSavePointMovable){
                Room * room = game->getRoomPtr();
                int platformToRemove = 0;
                int spikeToRemove = 0;
                int savePointToRemove = 0;
                bool isPlatformCollision = false;
                bool isSpikeCollision = false;
                bool isSavePointCollision = false;
                mouse.body.center[0] = e->xbutton.x;
                mouse.body.center[1] = WINDOW_HEIGHT - e->xbutton.y;
                for (unsigned int L = 0; L < room->savePoints.size(); L++){
                    if (collisionRectRect(&mouse.body,&room->savePoints[L]->body)){
                        savePointToRemove = L;
                        isSavePointCollision = true;
                    }
                }
                if (isSavePointCollision){
                    room->savePoints.erase(room->savePoints.begin() + savePointToRemove);
                    room->numSavePoints--;
                }
                if (!isSavePointCollision){
                    for (unsigned int k = 0; k < room->spikes.size(); k++){
                        if (collisionRectTri(&mouse.body,&room->spikes[k]->body)){
                            spikeToRemove = k;
                            isSpikeCollision = true;
                        }
                    }
                    if (isSpikeCollision){
                        room->spikes.erase(room->spikes.begin() + spikeToRemove);
                        room->numSpikes--;
                    }
                }

                if (!isSpikeCollision && !isSavePointCollision){
                    for (unsigned int p = 0; p < room->platforms.size(); p++){
                        if (collisionRectRect(&mouse.body,&room->platforms[p]->body)){
                            platformToRemove = p;
                            isPlatformCollision = true;
                        }
                    }
                    if (isPlatformCollision){
                        room->platforms.erase(room->platforms.begin() + platformToRemove);
                        room->numPlatforms--;
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
        //Room * room = game->getRoomPtr(); // UNUSED!
        mouse.body.center[0] = e->xbutton.x;
        mouse.body.center[1] = WINDOW_HEIGHT - e->xbutton.y;
        game->resizePlatform(&mouse);
    }
}

