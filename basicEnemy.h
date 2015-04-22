#ifndef BASICENEMY_H
#define BASICENEMY_H

#include "gameObject.h"


class basicEnemy : public gameObject
{
    public:
        Shape prevPosition;
        Vec velocity;
        int facing;
        int leftBoundary;
        int rightBoundary;
        basicEnemy();
        virtual ~basicEnemy();
    protected:
    private:
};

#endif // BASICENEMY_H
