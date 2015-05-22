#ifndef PLATFORM_H
#define PLATFORM_H

#include "gameObject.h"

class Platform : public GameObject
{
    public:
        Platform(int width, int height, int x, int y);
        Platform(GameObject*);
        virtual ~Platform();
        void update();
        string debugReport();
    protected:
    private:
};

#endif // PLATFORM_H
