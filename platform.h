#ifndef PLATFORM_H
#define PLATFORM_H

#include "gameObject.h"


class platform : public gameObject
{
    public:
        platform(int width, int height, int x, int y);
        virtual ~platform();
        void update();
    protected:
    private:
};

#endif // PLATFORM_H
