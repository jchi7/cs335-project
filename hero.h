#ifndef HERO_H
#define HERO_H

#include "gameObject.h"

class Hero : public GameObject
{
    public:
        CharacterState state;

        int leftPressed;
        int rightPressed;
        Vec prevPosition;
        Vec velocity;

        int numBullets;
        int maxBullets;
        int bulletVelocity;
        // These are the variables for the sprite sheets
        float Inc = .1; 
        Coordinates heroJump[10];
        Coordinates heroIdleR[10];
        Coordinates heroIdleL[10];
        Coordinates heroWalkingR[10];
        Coordinates heroWalkingL[10];
        Coordinates heroJumpR[10];
        Coordinates heroJumpL[10];


        // jumpInitiated is set to 1 when the jump key is pressed
        int jumpInitiated;
        int initialJump;
        int secondJump;
        int jumpCount;
        int jumpRelease;
        // jumpFinished is used to prevent the hero from double jumping or
        // jumping after falling off of a platform
        int jumpFinished;

        Hero();
        virtual ~Hero();

        void update();
        void movement();
        void onCollision(GameObject * obj);
    protected:
    private:
};

#endif // HERO_H
