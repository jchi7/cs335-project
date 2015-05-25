#ifndef GRAPHICCOMPONENT_H
#define GRAPHICCOMPONENT_H
#include "gameObject.h"

class GraphicComponent
{
    public:
        GraphicComponent();
        virtual ~GraphicComponent();
        virtual void update(GameObject* entity, int r, int g, int b);
    protected:
    private:
};

#endif // GRAPHICCOMPONENT_H
