#ifndef BASICENEMY_H
#define BASICENEMY_H

#include "gameObject.h"


class BasicEnemy : public GameObject
{
    public:
        Shape prevPosition;
        Vec velocity;
        int facing;
        int leftBoundary;
        int rightBoundary;
        BasicEnemy();
        virtual ~BasicEnemy();
    protected:
    private:
};

#endif // BASICENEMY_H
