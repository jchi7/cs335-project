#include "hero.h"
#include<iostream>

Hero::Hero()
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
    //setting up hero idle texture coordinates
    for ( int i = 0; i < 10; i++) {
        heroIdleR[i].x1 = 0.0;
        heroIdleR[i].x2 = 0.0;
        heroIdleR[i].y1 = 0.6;
        heroIdleR[i].y2 = 0.8;
        
        heroIdleL[i].x1 = 0.0;
        heroIdleL[i].x2 = 0.0;
        heroIdleL[i].y1 = 0.8;
        heroIdleL[i].y2 = 1.0;

        heroWalkingR[i].x1 = 0.0;
        heroWalkingR[i].x2 = 0.0;
        heroWalkingR[i].y1 = 0.2;
        heroWalkingR[i].y2 = 0.4;
        
        heroWalkingL[i].x1 = 0.0;
        heroWalkingL[i].x2 = 0.0;
        heroWalkingL[i].y1 = 0.4;
        heroWalkingL[i].y2 = 0.6;
    }
    for (int i = 0; i < 10; i++) {
        if(i == 0) {
            heroIdleR[i].x1 = heroIdleR[i].x2;
            heroIdleL[i].x1 = heroIdleL[i].x2;
            heroWalkingR[i].x1 = heroWalkingR[i].x2;
            heroWalkingL[i].x1 = heroWalkingL[i].x2;
        }
        else {
            heroIdleR[i].x1 = heroIdleR[i-1].x2;
            heroIdleL[i].x1 = heroIdleL[i-1].x2;
            heroWalkingR[i].x1 = heroWalkingR[i-1].x2;
            heroWalkingL[i].x1 = heroWalkingL[i-1].x2;
        }
        heroIdleR[i].x2 = idleIncrementor;
        heroIdleL[i].x2 = idleIncrementor;
        heroWalkingR[i].x2 = walkingIncrementor;
        heroWalkingL[i].x2 = walkingIncrementor;
        idleIncrementor += .068;
        walkingIncrementor += .0872;
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
void Hero::movement(){
    prevPosition[0] = body.center[0];
    prevPosition[1] = body.center[1];
    if (jumpRelease > 0)
        jumpRelease--;
    if (leftPressed == 1){
        body.center[0] += -3;
    }
    if (rightPressed == 1){
        body.center[0] += 3;
        std::cout<<"Walking Right\n";
    }
    if (initialJump == 1){
        velocity[1] = 7;
        state = JUMPING;
        initialJump = 0;
        jumpCount++;
    }
    if (secondJump == 1){
        velocity[1] = 7;
        state = JUMPING;
        secondJump = 0;
        jumpCount++;
    }
    body.center[1] += velocity[1];
    velocity[1] += gravity;
    if (prevPosition[1] > body.center[1] && (state == STANDING || state == WALKING)){
        state = JUMPING;
        jumpCount = 1;
    }

}
void Hero::onCollision(gameObject * platform){

    if (prevPosition[0]  < platform->body.center[0] - platform->body.width){
        body.center[0] = platform->body.center[0] - platform->body.width - body.width;
    }
    if (prevPosition[0]  > platform->body.center[0] + platform->body.width){
        body.center[0] = platform->body.center[0] + platform->body.width + body.width;
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
        jumpCount = 0;

        if (leftPressed == 1 || rightPressed == 1){
            state = WALKING;
        }
    }
}
