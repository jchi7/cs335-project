#ifndef BASICBULLET_H
#define BASICBULLET_H

#include "gameObject.h"

class BasicBullet : public GameObject
{
    public:
        CharacterState state;
        Vec prevPosition;
        Vec velocity;

        BasicBullet(int, int, int, int, ObjectType);
        virtual ~BasicBullet();
        void update();
        void movement();
        void onCollision(GameObject * obj);
        string debugReport();
    protected:
    private:
};

#endif // BASICBULLET_H
