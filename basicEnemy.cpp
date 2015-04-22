#include "basicEnemy.h"

basicEnemy::basicEnemy(int x, int y, int left, int right)
{
    this.body.width = 10;
    this.body.height = 20;
    this.body.center[0] = x;
    this.body.center[1] = y;
    this.body.center[2] = 0;
    this.velocity[0] = 0.5;
    this.leftBoundary = left;
    this.rightBoundary = right;
}

basicEnemy::~basicEnemy()
{
    //dtor
}
