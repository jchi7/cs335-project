#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "inputComponent.h"

enum ObjectType {HERO, ENEMY, SPIKE, PLATFORM};
class GameObject
{
    public:
        Shape body;
        ObjectType id;
        int rgb[3];
        float gravity;

        GameObject();
        virtual ~GameObject();
        virtual void update();
        virtual string debugReport();
    protected:
    private:
        InputComponent* IComponent;
};

#endif // GAMEOBJECT_H
