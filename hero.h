#ifndef HERO_H
#define HERO_H

#include <gameObject.h>


class hero : public gameObject
{
    public:
        Shape prevPosition;
        Vec velocity;
        int facing;
        int numBullets;
        int maxBullets;
        int bulletVelocity;
        hero();
        virtual ~hero();
    protected:
    private:
};

#endif // HERO_H
