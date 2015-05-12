#ifndef SAVEPOINT_H
#define SAVEPOINT_H

#include "gameObject.h"

class SavePoint : public GameObject
{
    public:
        SavePoint(int width, int height, int x, int y);
        virtual ~SavePoint();
        void update();
    protected:
    private:
};

#endif // SAVEPOINT_H
