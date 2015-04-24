#ifndef HERO_H
#define HERO_H

#include "gameObject.h"
enum CharacterState {STANDING, WALKING, JUMPING};
class Hero : public gameObject
{
    public:
        CharacterState state;        
        int leftPressed;
        int rightPressed;
        Vec prevPosition;
        Vec velocity;
        int facing;
        int numBullets;
        int maxBullets;
        // jumpInitiated is set to 1 when the jump key is pressed
        int jumpInitiated;
        int initialJump;
        int secondJump;
        int jumpCount;
        int jumpRelease;
        // jumpFinished is used to prevent the hero from double jumping or
        // jumping after falling off of a platform
        int jumpFinished;
        int bulletVelocity;
        // Temp for colors
        Hero();
        virtual ~Hero();
        void update();
        void movement();
        void onCollision(gameObject* platform);
    protected:
    private:
};

#endif // HERO_H
