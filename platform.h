#ifndef PLATFORM_H
#define PLATFORM_H

#include "gameObject.h"

class Platform : public GameObject
{
    public:
        Platform(int width, int height, int x, int y);
        virtual ~Platform();
        void update();
    protected:
    private:
};

#endif // PLATFORM_H
