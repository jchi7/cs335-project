#include "cannon.h"

Cannon::Cannon(int bulX, int bulY, int x, int y)
{
    this->body.type = RECTANGLE;
    this->id = CANNON;
    this->body.width = 12;
    this->body.height = 12;
    this->body.center[0] = x;
    this->body.center[1] = y;
    this->body.center[2] = 0;
    this->velocity[0] = bulX;
    this->velocity[1] = bulY;
    this->velocity[2] = 0;
    this->rgb[0] = 0;
    this->rgb[1] = 0;
    this->rgb[2] = 0;
    this->body.orientation = FACING_RIGHT;
    state = WALKING;
    Inc = .1;
}

Cannon::~Cannon()
{
    //dtor
}

void Cannon::update()
{
}

string Cannon::debugReport()
{
    ostringstream report;
    report << GameObject::debugReport()
      << "  ----"
      << "  SUBCLASS REPORT: Cannon\n"
      << "    Int Delay = "<< delay << endl;
    return report.str();
}

void Cannon::movement()
{

}

void Cannon::onCollision(GameObject * obj)
{
}
