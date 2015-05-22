#ifndef SPIKE_H
#define SPIKE_H

#include "gameObject.h"

class Spike : public GameObject
{
    public:
        Spike(Vec[3], ShapeOrientation);
        Spike(GameObject*);
        virtual ~Spike();
        void update();
        string debugReport();
    protected:
    private:
};

#endif // SPIKE_H
