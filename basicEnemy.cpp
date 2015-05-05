#include "basicEnemy.h"

BasicEnemy::BasicEnemy(int x, int y, int left, int right)
{
    this->body.type = RECTANGLE;
    this->id = ENEMY;
    this->body.width = 10;
    this->body.height = 20;
    this->body.center[0] = x;
    this->body.center[1] = y;
    this->body.center[2] = 0;
    this->prevPosition[0] = x;
    this->prevPosition[1] = y;
    this->prevPosition[2] = 0;
    this->velocity[0] = 0.5;
    this->velocity[1] = 0;
    this->velocity[2] = 0;
    this->facing = 1;
    this->leftBoundary = left;
    this->rightBoundary = right;
    this->rgb[0] = 150;
    this->rgb[1] = 100;
    this->rgb[2] = 100;
}

BasicEnemy::~BasicEnemy()
{
    //dtor
}

void BasicEnemy::update(GameObject* entity)
{
    //InputComponent->update(game, entity);
}

string BasicEnemy::debugReport()
{
    ostringstream report;
    report << GameObject::debugReport()
      << "  ----"
      << "  SUBCLASS REPORT: BasicEnemy\n"
      << "    Vec prevPosition = "<< vecPrint(this->prevPosition) << endl
      << "    Vec velocity = "<< vecPrint(this->velocity) << endl
      << "    int facing = "<< this->facing << endl
      << "    int leftBoundary = "<< this->leftBoundary << endl
      << "    int rightBoundary = "<< this->rightBoundary << endl;
    return report.str();
}
