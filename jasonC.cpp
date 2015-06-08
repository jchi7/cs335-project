#include "jasonC.h"
///////////////////////////////////////////////////////////////////////
//  Jason Chi
//  This files contains the enemies physics and the bullet physics
//  The physics includes edge detection and enemy behavior
///////////////////////////////////////////////////////////////////////
void enemyPhysics(Game *game)
{
    bool isCollision, isEdge, edge;
    Room * current_level = game->getRoomPtr();
    int i = (signed int) current_level->enemies.size() - 1;
    // Loop throught all the enemies
    for(; i >= 0; i--) {
        // If the enemy is current object in level editor
        if (game->isEnemyMovable && i == game->movableEnemyIndex)
            continue;
        BasicEnemy* entity = (BasicEnemy*) current_level->enemies[i];
        isCollision = false;
        isEdge = true;
        edge = true;
        entity->movement();
        // setup a box that a width ahead of the enemy for edge detection
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
            // Check ahead of the enemies and see if it is an edge.
            switch(entity->body.orientation) {
                case FACING_LEFT:
                    isEdge = collisionRectRect( &left, &current_level->platforms[i]->body);
                    if (isEdge) {
                        edge = false;
                    }
                    break;
                case FACING_RIGHT:
                    isEdge = collisionRectRect( &right, &current_level->platforms[i]->body);
                    if (isEdge) {
                        edge = false;
                    }
                    break;
                default:
                    break;
            }
            isCollision = collisionRectRect(&entity->body, &current_level->platforms[i]->body);
            if (isCollision == true) {
                entity->onCollision(current_level->platforms[i]);
            }
        }
        if (edge)
            entity->switchDirection();
        // Check ahead of the enemy for spikes
        isCollision = false;
        left.center[0] = entity->body.center[0] - entity->body.width;
        left.center[1] = entity->body.center[1];
        left.center[2] = entity->body.center[2];
        right.center[0] = entity->body.center[0] + entity->body.width;
        right.center[1] = entity->body.center[1];
        right.center[2] = entity->body.center[2];
        // check to see if the spike is ahead.  If so switch direction.
        // If the enemy collision with the spikes then do collision
        for (int i = 0; i < current_level->numSpikes; i++) {
            switch(entity->body.orientation) {
                case FACING_LEFT:
                    isCollision = collisionRectTri(&left, &current_level->spikes[i]->body);
                    if (isCollision == true) {
                        entity->switchDirection();
                    }
                    break;
                case FACING_RIGHT:
                    isCollision = collisionRectTri(&right, &current_level->spikes[i]->body);
                    if (isCollision == true) {
                        entity->switchDirection();
                    }
                    break;
                default:
                    break;
            }
            isCollision = collisionRectTri(&entity->body, &current_level->spikes[i]->body);
            if (isCollision == true) {
                entity->onCollision(current_level->spikes[i]);
            }
        }
        // check to see if the enemy collisions with the bullet
        for (int i = 0; i < current_level->numBullet; i++) {
            isCollision = collisionRectRect(&entity->body, &current_level->bullet[i]->body);
            if (current_level->bullet[i]->id == EBULLET || entity->state == PREDEATH)
                continue;
            if (isCollision == true) {
                entity->onCollision(current_level->bullet[i]);
                delete current_level->bullet[i];
                current_level->bullet.erase(current_level->bullet.begin() + i);
                current_level->numBullet--;
            }
        }
        
        if (entity->id == SHOOTERENEMY) {
            // set up a range detection to shoot at.
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
                    if (isCollision == true && entity->delay == 0) {
                        current_level->bullet.push_back(new BasicBullet(-4, 0, entity->body.center[0] - entity->body.width - 2, entity->body.center[1], ENEMY));
                        current_level->numBullet++;
                    }
                    entity->delay++;
                    break;
                case FACING_RIGHT:
                    isCollision = collisionRectRect(&right, &game->hero->body);
                    if (isCollision == true && entity->delay == 0) {
                        current_level->bullet.push_back(new BasicBullet(4, 0, entity->body.center[0] + entity->body.width + 2, entity->body.center[1], ENEMY));
                        current_level->numBullet++;
                    }
                    entity->delay++;
                    break;
                case STOP:
                default:
                    break;
            }
        }
        // Check to see if the enemy is going off screen and change direction
        if (entity->body.orientation == FACING_LEFT && (entity->body.center[0] - entity->body.width <= 0)) {
            entity->switchDirection();
        } else if (entity->body.orientation == FACING_RIGHT && (entity->body.center[0] + entity->body.width >= 1000)) {
            entity->switchDirection();
        }
        // If the enemy is in predeath then stop his action.
        // if the enemy is death then delete him
        switch (entity->state) {
            case PREDEATH:
                entity->body.orientation = STOP;
                break;
            case DEATH:
                delete current_level->enemies[i];
                current_level->enemies.erase(current_level->enemies.begin() + i);
                current_level->numBasicEnemies--;
                break;
            default:
                break;
        }
    } 
}
// This function does the bullet physics
void bulletPhysics(Game *game)
{
    bool isCollision;
    Room * current_level = game->getRoomPtr();
    int i = (signed int) current_level->bullet.size() - 1;

    for(; i >= 0; i--) {
        BasicBullet* entity = (BasicBullet*) current_level->bullet[i];
        isCollision = false;
        entity->movement();
        // Check to see if it collides with platforms
        for (int i = 0; i < current_level->numPlatforms; i++) {
            isCollision = collisionRectRect(&entity->body, &current_level->platforms[i]->body);
            if (isCollision == true) {
                entity->onCollision(current_level->platforms[i]);
            }
        }
        // Check to see if it collides with spikes
        isCollision = false;
        for (int i = 0; i < current_level->numSpikes; i++) {
            isCollision = collisionRectTri(&entity->body, &current_level->spikes[i]->body);
            if (isCollision == true) {
                entity->onCollision(current_level->spikes[i]);
            }
        }
        // delete bullet if it is offscreen
        if (entity->body.center[0] > 1000 || entity->body.center[0] <= 0)
            entity->state = DEATH;
        // delete bullets
        if (entity->state == DEATH) {
            delete current_level->bullet[i];
            current_level->bullet.erase(current_level->bullet.begin() + i);
            current_level->numBullet--;
        }
    }
}
