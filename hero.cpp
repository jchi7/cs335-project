#include "hero.h"

Hero::Hero()
{
    numBullets = 0;
    maxBullets = 3;
    bulletVelocity = 10;
    delay = 0;

    body.type = RECTANGLE;
    id = HERO;
    body.width = 8;
    body.height = 15;
    vecMake(400, 250, body.center);
    vecCopy(body.center, prevPosition);
    vecZero(velocity);

    body.orientation = FACING_RIGHT;

    bottomPlatformCollision = false;

    body.center[0] = 50;
    body.center[1] = 100;
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

        /*heroDeath[i].x1 = 0.0;
        heroDeath[i].x2 = 0.0;
        heroDeath[i].y1 = 0.0;
        heroDeath[i].y2 = 1.0;
        */
    }

//The following code is importing the textures of the sprite into arrays.
    Inc = .1;
    for(int i = 0; i < 10; i++) {
        if(i == 0) {
            heroIdleR[i].x1 = heroIdleR[i].x2;
            heroIdleL[i].x1 = heroIdleL[i].x2;
            heroWalkingR[i].x1 = heroWalkingR[i].x2;
            heroWalkingL[i].x1 = heroWalkingL[i].x2;
            heroJumpR[i].x1 = heroJumpR[i].x2;
            heroJumpL[i].x1 = heroJumpL[i].x2;
            //heroDeath[i].x1 = heroDeath[i].x2;
        }
        else {
            heroIdleR[i].x1 = heroIdleR[i-1].x2;
            heroIdleL[i].x1 = heroIdleL[i-1].x2;
            
            heroWalkingR[i].x1 = heroWalkingR[i-1].x2;
            heroWalkingL[i].x1 = heroWalkingL[i-1].x2;

            heroJumpR[i].x1 = heroJumpR[i - 1].x2;
            heroJumpL[i].x1 = heroJumpL[i - 1].x2;
            //heroDeath[i].x1 = heroDeath[i-1].x2;
        }
        heroJumpR[i].x2 = Inc;
        heroJumpL[i].x2 = Inc;
        heroIdleR[i].x2 = Inc;
        heroIdleL[i].x2 = Inc;
        heroWalkingR[i].x2 = Inc;
        heroWalkingL[i].x2 = Inc;
        //heroDeath[i].x2 = Inc;
        Inc += .1;
    }
    //This is just a static image so I am mapping all four corners.
    heroDeath[0].x1 = 0.0; 
    heroDeath[0].x2 = 1.0;
    heroDeath[0].y1 = 0.0;
    heroDeath[0].y2 = 1.0;

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
    if(state != DEATH) {
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
			playJump();
        }
        if (secondJump == 1) {
            velocity[1] = 5.5;
            state = JUMPING;
            secondJump = 0;
            jumpCount++;
			playJump();
        }
    }
    body.center[1] += velocity[1];
    if (velocity[1] > -7)
        velocity[1] += gravity;
    if (jumpRelease == 3 && velocity[1] > 2 && velocity[1] < 5.5)
        velocity[1] = 1;
    if (prevPosition[1] > (body.center[1] + 2) && (state == STANDING || state == WALKING)){
        state = JUMPING;
        jumpCount = 1;
    }
}

// reposition hero & reset hero state
void Hero::onCollision(GameObject * obj)
{
    if (obj->id == SPIKE || obj->id == ENEMY || obj->id == EBULLET) {
            if (((BasicEnemy*)obj)->state != PREDEATH  || ((ShooterEnemy*)obj)->state != PREDEATH) {
                state = DEATH;
                return;
            }
    }
    else if (obj->id == HBULLET) {
        return;
    }
    else { // obj->id == PLATFORM || ELEVATOR
        Vec prevVel;
        vecCopy(velocity, prevVel);
        if (prevPosition[0]  < obj->body.center[0] - obj->body.width) {
            if (prevPosition[1] - body.height < obj->body.center[1] + obj->body.height){
                body.center[0] = obj->body.center[0] - obj->body.width - body.width;
                return;
            }
        }

        if (prevPosition[0]  > obj->body.center[0] + obj->body.width) {
            if (prevPosition[1] - body.height < obj->body.center[1] + obj->body.height){
                body.center[0] = obj->body.center[0] + obj->body.width + body.width;
                return;
            }
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
            if (obj->id != ELEVATOR) {
                velocity[1] = 0;
            }
            jumpCount = 0;
            
            if (state == DEATH)
                return;
            if (leftPressed == 1 || rightPressed == 1) {
                state = WALKING;
            }
            else {
                state = STANDING;
            }
        }
        if (obj->id == ELEVATOR) {
            if (obj->velocity[1] >= 0) {
                velocity[1]=0;
                return;
            }

//            if (topOnlyCollisionRectRect(&body, &(obj->body), prevPosition))
//            {
//                velocity[1] += obj->velocity[1];
//                // DEBUG:
//                //cout << "top collision, velocity[1] = " << velocity[1] << endl;
//            }
//            else {
//                // DEBUG:
//                //cout << "non-top collision, velocity[1] = " << velocity[1] << endl;
            if (body.center[1] <= obj->body.center[1]+obj->body.height) {
              velocity[1] += obj->velocity[1] * ((Elevator*)obj)->getVertSpeed();
              body.center[1] += velocity[1];
              
            }
            else
            {
                //platform slower than heros velocity
                if(obj->velocity[1] >= velocity[1]) {
                    velocity[1] = obj->velocity[1] * ((Elevator*)obj)->getVertSpeed();
                }

                //platform down is faster than hero 
                else {
                }
            }
            //velocity[1] = prevVel[1];
//            }
        }
    }
}

