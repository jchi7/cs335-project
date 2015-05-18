#include "jasonc.h"
//  Jason Chi
//  This files does edge detection for the enemies
/////////////////////////////////////////////////
void enemyPhysics(Game *game)
{
    bool isCollision, isEdge, edge;
    Room * current_level = game->getRoomPtr();
    int i = (signed int) current_level->enemies.size() - 1;

    for(; i >= 0; i--) {
    //for(auto &entity : current_level->enemies) {
        BasicEnemy* entity = (BasicEnemy*) current_level->enemies[i];
        isCollision = false;
        isEdge = true;
        edge = true;
        entity->movement();
        Shape left;
        Shape right;
        left.type = RECTANGLE;
        left.center[0] = entity->body.center[0] - 2 * entity->body.width;
        left.center[1] = entity->body.center[1] - 0.1;
        left.center[2] = entity->body.center[2];
        left.width = entity->body.width;
        left.height = entity->body.height;
        right.type = RECTANGLE;
        right.center[0] = entity->body.center[0] + 2 * entity->body.width;
        right.center[1] = entity->body.center[1] - 0.1;
        right.center[2] = entity->body.center[2];
        right.width = entity->body.width;
        right.height = entity->body.height;
        for (int i = 0; i < current_level->numPlatforms; i++) {
            if (entity->body.orientation == FACING_LEFT) {
                isEdge = collisionRectRect( &left, &current_level->platforms[i]->body);
                if (isEdge) {
                    edge = false;
                }
            } else if (entity->body.orientation == FACING_RIGHT) {
                isEdge = collisionRectRect( &right, &current_level->platforms[i]->body);
                if (isEdge) {
                    edge = false;
                }
            }
            isCollision = collisionRectRect(&entity->body, &current_level->platforms[i]->body);
            if (isCollision == true) {
                entity->onCollision(current_level->platforms[i]);
            }
        }
        if (edge)
            entity->switchDirection();
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
        if (entity->state == DEATH) {
            current_level->enemies.erase(current_level->enemies.begin() + i);
            current_level->numBasicEnemies--;
        }
    } 
}
