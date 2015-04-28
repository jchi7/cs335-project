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

graphicComponent::renderBackground(GLuint backgroundTexture)
{    
    PushhMatrix();
    glBindTexture(GL_TEXTURE_2D,backgroundTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,1.0f); glVertex2i(0,0);
    glTexCoord2f(0.0,0.0); glVertex2i(0,WINDOW_HEIGHT);
    glTexCoord2f(1.0,0.0); glVertex2i(WINDOW_WIDTH,WINDOW_HEIGHT);
    glTexCoord2f(1.0f,1.0f); glVertex2i(WINDOW_WIDTH,0);
    glPopMatrix();
    glEnd();
}

