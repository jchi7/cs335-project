#ifndef SPIKE_H
#define SPIKE_H

#include "gameObject.h"

class Spike : public GameObject
{
    public:
        Spike(Vec p[3], int orientation);
        virtual ~Spike();
        void update();
    protected:
    private:
};

#endif // SPIKE_H
