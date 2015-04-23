#include "hero.h"

hero::hero()
{
    numBullets = 0;
    maxBullets = 3;
    bulletVelocity = 10;
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
    jumpFinished = 0;
    rgb[0] = 200;
    rgb[1] = 200;
    rgb[2] = 200;
    leftPressed = 0;
    rightPressed = 0;
    state = STANDING;

}

hero::~hero()
{
    //dtor
}

void hero::update()
{

}
void hero::movement(){
    if (leftPressed == 1){
        body.center[0] += -3;
    }
    if (rightPressed == 1){
        body.center[0] += 3;
    }
    if (jumpInitiated == 1){
        velocity[1] = 8;
        state = JUMPING;
        jumpInitiated = 0;
    }
    body.center[1] += velocity[1];
    velocity[1] += gravity;
    prevPosition[0] = body.center[0];
    prevPosition[1] = body.center[1];

}
void hero::onCollision(gameObject * platform){

    if (prevPosition[0] + body.width < platform->body.center[0] - platform->body.width){
        body.center[0] = platform->body.center[0] - platform->body.width - body.width - 10;
    }

    if (body.center[1] < platform->body.center[1] &&
            body.center[0] + body.width > platform->body.center[0] - platform->body.width &&
            body.center[0] - body.width < platform->body.center[0] + platform->body.width 
            ){
        body.center[1] = platform->body.center[1] - platform->body.height - body.height;
        velocity[1] = 0;
    }

    if (body.center[1] > platform->body.center[1] &&
            body.center[0] + body.width > platform->body.center[0] - platform->body.width &&
            body.center[0] - body.width < platform->body.center[0] + platform->body.width 
            ){
        body.center[1] = platform->body.center[1] + platform->body.height + body.height;
        velocity[1] = 0;
        state = STANDING;
            if (leftPressed == 1){
            state = WALKING;
        }
        if (rightPressed == 1){
            state = WALKING;
        }
    }
}
