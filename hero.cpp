#include "hero.h"

Hero::Hero()
{
    numBullets = 0;
    maxBullets = 3;
    bulletVelocity = 10;

    body.type = RECTANGLE;
    id = HERO;
    body.width = 8;
    body.height = 15;
    body.center[0] = 400;
    body.center[1] = 250;
    body.center[2] = 0;
    prevPosition[0] = 400;
    prevPosition[1] = 250;
    velocity[0] = 0;
    velocity[1] = 0;
    facing = 1;

    jumpInitiated = 0;
    initialJump = 0;
    secondJump = 0;
    jumpCount = 0;
    jumpRelease = 1;
    jumpFinished = 0;

    rgb[0] = 200;
    rgb[1] = 200;
    rgb[2] = 200;
    leftPressed = 0;
    rightPressed = 0;
    state = STANDING;
}

Hero::~Hero()
{
    //dtor
}

void Hero::update()
{

}

void Hero::movement()
{
    prevPosition[0] = body.center[0];
    prevPosition[1] = body.center[1];
    if (jumpRelease > 0) {
        jumpRelease--;
    }
    if (leftPressed == 1) {
        body.center[0] += -3;
    }
    if (rightPressed == 1) {
        body.center[0] += 3;
    }
    if (initialJump == 1){
        velocity[1] = 7;
        state = JUMPING;
        initialJump = 0;
        jumpCount++;
    }
    if (secondJump == 1) {
        velocity[1] = 7;
        state = JUMPING;
        secondJump = 0;
        jumpCount++;
    }
    body.center[1] += velocity[1];
    velocity[1] += gravity;
    if (prevPosition[1] > body.center[1] && (state == STANDING || state == WALKING)) {
        state = JUMPING;
        jumpCount = 1;
    }
}

// reposition hero & reset hero state
void Hero::onCollision(GameObject * obj)
{
    if (obj->id == SPIKE) {
        state = DEATH;
    }
    else { // obj->id == PLATFORM
        if (prevPosition[0]  < obj->body.center[0] - obj->body.width) {
            body.center[0] = obj->body.center[0] - obj->body.width - body.width;
        }

        if (prevPosition[0]  > obj->body.center[0] + obj->body.width) {
            body.center[0] = obj->body.center[0] + obj->body.width + body.width;
        }

        if (body.center[1] < obj->body.center[1] &&
            body.center[0] + body.width > obj->body.center[0] - obj->body.width &&
            body.center[0] - body.width < obj->body.center[0] + obj->body.width)
        {
            body.center[1] = obj->body.center[1] - obj->body.height - body.height;
            velocity[1] = 0;
        }

        if (body.center[1] > obj->body.center[1] &&
            body.center[0] + body.width > obj->body.center[0] - obj->body.width &&
            body.center[0] - body.width < obj->body.center[0] + obj->body.width)
        {
            body.center[1] = obj->body.center[1] + obj->body.height + body.height;
            velocity[1] = 0;
            jumpCount = 0;

            if (leftPressed == 1 || rightPressed == 1) {
                state = WALKING;
            }
            else {
                state = STANDING;
            }
        }
    }
}

