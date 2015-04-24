#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "InputComponent.h"

class gameObject
{
    public:
        Shape body;
        int rgb[3];
        float gravity;
        gameObject();
        virtual ~gameObject();
        virtual void update();
    protected:
    private:
        InputComponent* IComponent;
};

#endif // GAMEOBJECT_H
