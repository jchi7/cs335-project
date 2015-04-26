#ifndef PLATFORM_H
#define PLATFORM_H

#include "gameObject.h"

class Platform : public GameObject
{
    public:
        enum platformType {
            GROUND,
            SPIKE
        };
        platformType type;

        Platform(int width, int height, int x, int y, const char * ptype);
        virtual ~Platform();
        void update();
    protected:
    private:
};

#endif // PLATFORM_H
