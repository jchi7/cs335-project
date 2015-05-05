#ifndef BASICENEMY_H
#define BASICENEMY_H

#include "gameObject.h"


class BasicEnemy : public GameObject
{
    public:
        Vec prevPosition;
        Vec velocity;
        int facing;
        int leftBoundary;
        int rightBoundary;

        BasicEnemy(int, int, int, int);
        virtual ~BasicEnemy();
        void update(GameObject*);
        string debugReport();
    protected:
    private:
};

#endif // BASICENEMY_H
