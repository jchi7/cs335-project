#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include "gameObject.h"

class SpawnPoint : public GameObject
{
    public:
        SpawnPoint( int x, int y, ObjectType type);
        virtual ~SpawnPoint();
        void update();
    protected:
    private:
};

#endif // SPAWNPOINT_H
