#include "jasonc.h"
//  Jason Chi
//  This files does edge detection for the enemies
/////////////////////////////////////////////////
//  This is function is
//  Arguments:  Take an entity and a platform.
/////////////////////////////////////////////////
bool entityPeakAheadDetect(GameObject& ent, Shape* plat)
{
    if ((ent.body.center[0] + ent.body.height) <= (plat->center[0] + plat->height))
        return false;
    // if near the left edge then you to switch direction
    if (ent.body.orientation == FACING_RIGHT) {
       if (ent.body.center[0] + ent.body.width >= plat->center[0] - plat->width &&
           ent.body.center[0] + ent.body.width <= plat->center[0] + plat->width &&
           ent.body.center[1] - ent.body.height < plat->center[1] + plat->height &&
           ent.body.center[1] + ent.body.height > plat->center[1] - plat->height ) {
           return false;
        } 
    } else if (ent.body.orientation == FACING_LEFT) {
        if (ent.body.center[0] - ent.body.width >= plat->center[0] - plat->width &&
            ent.body.center[0] - ent.body.width <= plat->center[0] + plat->width &&
            ent.body.center[1] - ent.body.height < plat->center[1] + plat->height &&
            ent.body.center[1] + ent.body.height > plat->center[1] - plat->height ) {
            return false;
        }
    }
    return true;
}

void enemyPhysics(Game *game)
{
    bool isCollision, isEdge;
    Room * current_level = game->getRoomPtr();

    for(auto &entity : current_level->enemies) {
        isCollision = false;
        isEdge = false;
        entity->movement();
        Shape left;
        Shape right;
        left.type = RECTANGLE;
        left.center[0] = entity->body.center[0] - entity->body.width - 0.5;
        left.center[1] = entity->body.center[1];
        left.center[2] = entity->body.center[2];
        left.width = entity->body.width;
        left.height = entity->body.height;
        right.type = RECTANGLE;
        right.center[0] = entity->body.center[0] + entity->body.width + 0.5;
        right.center[1] = entity->body.center[1];
        right.center[2] = entity->body.center[2];
        right.width = entity->body.width;
        right.height = entity->body.height;
        for (int i = 0; i < current_level->numPlatforms; i++) {
            /*if (entityPeakAheadDetect(*entity, &current_level->platforms[i]->body)) {
                isEdge = true;
            }*/
            if (entity->body.orientation == FACING_LEFT) {
                isEdge = collisionRectRect( &left, &current_level->platforms[i]->body);
                if (isEdge == false) {
                    //entity->switchDirection();
                }
            } else if (entity->body.orientation == FACING_RIGHT) {
                isEdge = collisionRectRect( &right, &current_level->platforms[i]->body);
                if (isEdge == false) {
                    //entity->switchDirection();
                }
            }
            isCollision = collisionRectRect(&entity->body, &current_level->platforms[i]->body);
            if (isCollision == true) {
                entity->onCollision(current_level->platforms[i]);
            }
        }
        isCollision = false;
        
        for (int i = 0; i < current_level->numSpikes; i++) {
            isCollision = collisionRectTri(&left, &current_level->spikes[i]->body);
            if (isCollision == true) {
                entity->switchDirection();
            }
            isCollision = collisionRectTri(&right, &current_level->spikes[i]->body);
            if (isCollision == true) {
                entity->switchDirection();
            }
            isCollision = collisionRectTri(&entity->body, &current_level->spikes[i]->body);
            if (isCollision == true) {
                entity->onCollision(current_level->spikes[i]);
            }
        }
        if (entity->body.orientation == FACING_LEFT && (entity->body.center[0] - entity->body.width <= 0)) {
            entity->switchDirection();
        } else if (entity->body.orientation == FACING_RIGHT && (entity->body.center[0] + entity->body.width >= 1000)) {
            entity->switchDirection();
        }
        /*if (game->hero->state == DEATH) {
            // TEMPORARY: return hero to start
            game->hero->jumpInitiated = 0;
            game->hero->initialJump = 0;
            game->hero->secondJump = 0;
            game->hero->jumpCount = 0;
            game->hero->jumpRelease = 1;
            game->hero->jumpFinished = 0;
            game->hero->velocity[0] = 0;
            //game->hero->velocity[1] = 0;
        }*/
    } 
}
