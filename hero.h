#ifndef HERO_H
#define HERO_H

#include "gameObject.h"


class hero : public gameObject
{
    public:
        int leftPressed;
        int rightPressed;
        Vec prevPosition;
        Vec velocity;
        int facing;
        int numBullets;
        int maxBullets;
        // jumpInitiated is set to 1 when the jump key is pressed
        int jumpInitiated;
        // jumpFinished is used to prevent the hero from double jumping or
        // jumping after falling off of a platform
        int jumpFinished;
        int bulletVelocity;
        // Temp for colors
        hero();
        virtual ~hero();
        void update();
        void movement();
    protected:
    private:
};

#endif // HERO_H
