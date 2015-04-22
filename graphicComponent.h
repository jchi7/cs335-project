#ifndef GRAPHICCOMPONENT_H
#define GRAPHICCOMPONENT_H
#include "gameObject.h"

class graphicComponent
{
    public:
        graphicComponent();
        virtual ~graphicComponent();
        virtual void update(gameObject* entity, int r, int g, int b);
    protected:
    private:
};

#endif // GRAPHICCOMPONENT_H
