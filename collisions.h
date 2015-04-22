#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "gameObject.h"


class collisions
{
    public:
        collisions();

    static bool collision(gameObject* entity1, gameObject* entity2);
};

#endif // COLLISIONS_H
