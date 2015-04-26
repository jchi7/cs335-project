#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "InputComponent.h"

class GameObject
{
    public:
        Shape body;
        int rgb[3];
        float gravity;
        GameObject();
        virtual ~GameObject();
        virtual void update();
    protected:
    private:
        InputComponent* IComponent;
};

#endif // GAMEOBJECT_H
