#ifndef CANNON_H
#define CANNON_H

#include "basicEnemy.h"


class Cannon : public BasicEnemy
{
    public:
        CharacterState state;
        Vec velocity;
        int delay;
    	float Inc;
	
        Cannon() {}
        Cannon(int, int, int, int);
        virtual ~Cannon();
        virtual void update();
        virtual void movement();
        virtual void onCollision(GameObject * obj);
        string debugReport();
    protected:
    private:
};

#endif // CANNON_H
