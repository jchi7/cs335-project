#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "structs.h"

class GameObject
{
    public:
        Shape body;
        ObjectType id;
        int tex_id;

        Vec prevPosition;
        Vec velocity;
        float gravity;

        int rgb[3];
        int textureWidth;
        int textureHeight;
        int horizontalTiles;
        int verticalTiles;

        GameObject();
        virtual ~GameObject();
        virtual void update();
        virtual void movement();
        virtual void onCollision();
        virtual string debugReport();
        virtual void switchDirection();
    protected:
    private:
};

#endif // GAMEOBJECT_H
