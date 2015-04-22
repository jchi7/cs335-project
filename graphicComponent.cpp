#include "graphicComponent.h"

graphicComponent::graphicComponent()
{
    //ctor
}

graphicComponent::~graphicComponent()
{
    //dtor
}

graphicComponent::update(gameObject* entity , int r, int g, int b)
{
    glColor3ub(r, g, b);
    glPushMatrix();
    glTranslatef(entity->body.center[0], entity->body.center[1], entity->body.center[2]);
    glBegin(GL_QUADS);
    glVertex2i(-entity->body.width,-entity->body.height);
    glVertex2i(-entity->body.width,entity->body.height);
    glVertex2i(entity->body.width,-entity->body.height);
    glVertex2i(entity->body.width,entity->body.height);
    glEnd();
    glPopMatrix();
}
