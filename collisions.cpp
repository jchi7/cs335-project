#include "collisions.h"

bool collision(GameObject* entity1, GameObject* entity2)
{
    if (entity1->body.center[0] + entity1->body.width >= entity2->body.center[0] - entity2->body.width &&
                entity1->body.center[0] - entity1->body.width <= entity2->body.center[0] + entity2->body.width &&
                entity1->body.center[1] - entity1->body.height < entity2->body.center[1] + entity2->body.height &&
                entity1->body.center[1] + entity1->body.height > entity2->body.center[1] - entity2->body.height )
        {
            return true;
        }
        return false;
       
}
