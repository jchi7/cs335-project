#include "gameObject.h"


gameObject::gameObject()
{
    this.GComponent = new graphicComponent();
}

gameObject::~gameObject()
{
    //dtor
    delete GComponent;
}

gameObject::update()
{
    this.GComponent->update(*this, 200, 200, 200);
}
