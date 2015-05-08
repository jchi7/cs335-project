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

    body.orientation = FACING_RIGHT;

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


    for( int i = 0; i < 10; i++) {
        heroIdleR[i].x1 = 0.0;
        heroIdleR[i].x2 = 0.0;
        heroIdleR[i].y1 = 0.0;
        heroIdleR[i].y2 = 1.0;

        heroIdleL[i].x1 = 0.0;
        heroIdleL[i].x2 = 0.0;
        heroIdleL[i].y1 = 0.0;
        heroIdleL[i].y2 = 1.0;

        heroWalkingR[i].x1 = 0.0;
        heroWalkingR[i].x2 = 0.0;
        heroWalkingR[i].y1 = 0.0;//prev .6
        heroWalkingR[i].y2 = 1.0;//prev .8

        heroWalkingL[i].x1 = 0.0;
        heroWalkingL[i].x2 = 0.0;
        heroWalkingL[i].y1 = 0.0;//prev .4
        heroWalkingL[i].y2 = 1.0;//prev .6

        heroJumpR[i].x1 = 0.0;
        heroJumpR[i].x2 = 0.0;
        heroJumpR[i].y1 = 0.0;
        heroJumpR[i].y2 = 1.0;
        
        heroJumpL[i].x1 = 0.0;
        heroJumpL[i].x2 = 0.0;
        heroJumpL[i].y1 = 0.0;
        heroJumpL[i].y2 = 1.0;
    }

//The following code is importing the textures of the sprite into arrays.
    for(int i = 0; i < 10; i++) {
        if(i == 0) {
            heroIdleR[i].x1 = heroIdleR[i].x2;
            heroIdleL[i].x1 = heroIdleL[i].x2;
            heroWalkingR[i].x1 = heroWalkingR[i].x2;
            heroWalkingL[i].x1 = heroWalkingL[i].x2;
            heroJumpR[i].x1 = heroJumpR[i].x2;
            heroJumpL[i].x1 = heroJumpL[i].x2;
        }
        else {
            heroIdleR[i].x1 = heroIdleR[i-1].x2;
            heroIdleL[i].x1 = heroIdleL[i-1].x2;
            
            heroWalkingR[i].x1 = heroWalkingR[i-1].x2;
            heroWalkingL[i].x1 = heroWalkingL[i-1].x2;

            heroJumpR[i].x1 = heroJumpR[i - 1].x2;
            heroJumpL[i].x1 = heroJumpL[i - 1].x2;
        }
        heroJumpR[i].x2 = Inc;
        heroJumpL[i].x2 = Inc;
        heroIdleR[i].x2 = Inc;
        heroIdleL[i].x2 = Inc;
        heroWalkingR[i].x2 = Inc;
        heroWalkingL[i].x2 = Inc;
        Inc += .1;
    }

      //Setting up the hero jump texture coordinates.
    heroJump[0].x1 = 0.0;
    heroJump[0].x2 = 0.07;
    heroJump[0].y1 = 0.0;
    heroJump[0].y2 = 0.2;

    heroJump[1].x1 = 0.1;
    heroJump[1].x2 = 0.17;
    heroJump[1].y1 = 0.0;
    heroJump[1].y2 = 0.2;

    heroJump[2].x1 = 0.2;
    heroJump[2].x2 = 0.28;
    heroJump[2].y1 = 0.0;
    heroJump[2].y2 = 0.2;

    heroJump[3].x1 = 0.3;
    heroJump[3].x2 = 0.38;
    heroJump[3].y1 = 0.0;
    heroJump[3].y2 = 0.2;

    heroJump[4].x1 = 0.4;
    heroJump[4].x2 = 0.49;
    heroJump[4].y1 = 0.0;
    heroJump[4].y2 = 0.2;


    heroJump[5].x1 = 0.5;
    heroJump[5].x2 = 0.59;
    heroJump[5].y1 = 0.0;
    heroJump[5].y2 = 0.2;

    heroJump[6].x1 = 0.6;
    heroJump[6].x2 = 0.69;
    heroJump[6].y1 = 0.0;
    heroJump[6].y2 = 0.2;

    heroJump[7].x1 = 0.7;
    heroJump[7].x2 = 0.8;
    heroJump[7].y1 = 0.0;
    heroJump[7].y2 = 0.2;

    heroJump[8].x1 = 0.8;
    heroJump[8].x2 = 0.9;
    heroJump[8].y1 = 0.0;
    heroJump[8].y2 = 0.2;


    heroJump[9].x1 = 0.9;
    heroJump[9].x2 = 1.0;
    heroJump[9].y1 = 0.0;
    heroJump[9].y2 = 0.2;


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
        body.orientation = FACING_LEFT;
        body.center[0] += -3;
    }
    if (rightPressed == 1) {
        body.orientation = FACING_RIGHT;
        body.center[0] += 3;
    }
    if (initialJump == 1){
        velocity[1] = 5.5;
        state = JUMPING;
        initialJump = 0;
        jumpCount++;
    }
    if (secondJump == 1) {
        velocity[1] = 5.5;
        state = JUMPING;
        secondJump = 0;
        jumpCount++;
    }
    body.center[1] += velocity[1];
    if (velocity[1] > -7)
        velocity[1] += gravity;
    if (prevPosition[1] > (body.center[1] + 2) && (state == STANDING || state == WALKING)){
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

