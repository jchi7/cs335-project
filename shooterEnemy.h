#ifndef SHOOTERENEMY_H
#define SHOOTERENEMY_H

#include "gameObject.h"
#include "basicEnemy.h"


class ShooterEnemy : public BasicEnemy
{
    public:
        ShooterEnemy( float, float);
        Coordinates sprite[10];
        float Inc;
        virtual ~ShooterEnemy();
        virtual void update();
        virtual void movement();
        virtual void onCollision(GameObject * obj);
        virtual void switchDirection();
        string debugReport();
    protected:
    private:
};

#endif // SHOOTERENEMY_H
