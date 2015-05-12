#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "inputComponent.h"

class GameObject
{
    public:
        Shape body;
        ObjectType id;
        int rgb[3];
        float gravity;
        int textureWidth;
        int textureHeight;
        int horizontalTiles;
        int verticalTiles;
        GameObject();
        virtual ~GameObject();
        virtual void update();
        virtual string debugReport();
    protected:
    private:
        InputComponent* IComponent;
};

#endif // GAMEOBJECT_H
