#ifndef BASICENEMY_H
#define BASICENEMY_H

#include "gameObject.h"


class BasicEnemy : public GameObject
{
    public:
        CharacterState state;
        Vec prevPosition;
        Vec velocity;
        int delay;
	float Inc;
	Coordinates enemyWalkRight[10];
	Coordinates enemyWalkLeft[10];

        BasicEnemy() {}
        BasicEnemy(int, int, int, int);
        virtual ~BasicEnemy();
        virtual void update();
        virtual void movement();
        virtual void onCollision(GameObject * obj);
        virtual void switchDirection();
        string debugReport();
    protected:
    private:
};

#endif // BASICENEMY_H
