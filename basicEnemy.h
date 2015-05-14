#ifndef BASICENEMY_H
#define BASICENEMY_H

#include "gameObject.h"


class BasicEnemy : public GameObject
{
    public:
        CharacterState state;
        Vec prevPosition;
        Vec velocity;
        int leftBoundary;
        int rightBoundary;

        BasicEnemy(int, int, int, int);
        virtual ~BasicEnemy();
        void update();
        void movement();
        void onCollision(GameObject * obj);
        void switchDirection();
        string debugReport();
    protected:
    private:
};

#endif // BASICENEMY_H
