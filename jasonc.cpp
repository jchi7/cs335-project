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
        left.center[0] = entity->body.center[0] - entity->body.width;
        left.center[1] = entity->body.center[1];
        left.center[2] = entity->body.center[2];
        right.center[0] = entity->body.center[0] + entity->body.width;
        right.center[1] = entity->body.center[1];
        right.center[2] = entity->body.center[2];
        
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
        for (int i = 0; i < current_level->numBullet; i++) {
            isCollision = collisionRectRect(&entity->body, &current_level->bullet[i]->body);
            if (isCollision == true) {
                entity->onCollision(current_level->bullet[i]);
                current_level->bullet.erase(current_level->bullet.begin() + i);
                current_level->numBullet--;
            }
        }
        
        if (entity->id == SHOOTERENEMY) {
            left.width = 10 * entity->body.width;
            right.width = 10 * entity->body.width;
            left.center[0] = entity->body.center[0] - left.width;
            left.center[1] = entity->body.center[1];
            left.center[2] = entity->body.center[2];
            right.center[0] = entity->body.center[0] + right.width;
            right.center[1] = entity->body.center[1];
            right.center[2] = entity->body.center[2];
            isCollision = false;
            entity->delay = entity->delay % 40;
            switch(entity->body.orientation) {
                case FACING_LEFT:
                    isCollision = collisionRectRect(&left, &game->hero->body);
                    //entity->delay = entity->delay % 40;
                    if (isCollision == true && entity->delay == 0) {
                        current_level->bullet.push_back(new BasicBullet(-4, 0, entity->body.center[0] - entity->body.width - 2, entity->body.center[1], ENEMY));
                        current_level->numBullet++;
                    }
                    entity->delay++;
                    break;
                case FACING_RIGHT:
                    isCollision = collisionRectRect(&right, &game->hero->body);
                    //entity->delay = entity->delay % 40;
                    if (isCollision == true && entity->delay == 0) {
                        current_level->bullet.push_back(new BasicBullet(4, 0, entity->body.center[0] + entity->body.width + 2, entity->body.center[1], ENEMY));
                        current_level->numBullet++;
                    }
                    entity->delay++;
                    break;
                default:
                    break;
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

void bulletPhysics(Game *game)
{
    bool isCollision;
    Room * current_level = game->getRoomPtr();
    int i = (signed int) current_level->bullet.size() - 1;

    for(; i >= 0; i--) {
        BasicBullet* entity = (BasicBullet*) current_level->bullet[i];
        isCollision = false;
        entity->movement();
        for (int i = 0; i < current_level->numPlatforms; i++) {
            isCollision = collisionRectRect(&entity->body, &current_level->platforms[i]->body);
            if (isCollision == true) {
                entity->onCollision(current_level->platforms[i]);
            }
        }
        isCollision = false;
        for (int i = 0; i < current_level->numSpikes; i++) {
            isCollision = collisionRectTri(&entity->body, &current_level->spikes[i]->body);
            if (isCollision == true) {
                entity->onCollision(current_level->spikes[i]);
            }
        }
        if (entity->state == DEATH) {
            current_level->bullet.erase(current_level->bullet.begin() + i);
            current_level->numBullet--;
        }
    }
}
