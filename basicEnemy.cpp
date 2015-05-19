#include "basicEnemy.h"

BasicEnemy::BasicEnemy(int left, int right, int x, int y)
{
    this->body.type = RECTANGLE;
    this->id = ENEMY;
    this->body.width = 10;
    this->body.height = 15;
    this->body.center[0] = x;
    this->body.center[1] = y;
    this->body.center[2] = 0;
    this->prevPosition[0] = x;
    this->prevPosition[1] = y;
    this->prevPosition[2] = 0;
    this->velocity[0] = 0.5;
    this->velocity[1] = 0;
    this->velocity[2] = 0;
    this->leftBoundary = left;
    this->rightBoundary = right;
    this->rgb[0] = 0;
    this->rgb[1] = 0;
    this->rgb[2] = 0;
    this->body.orientation = FACING_RIGHT;
    state = WALKING;
    Inc = .1;
    for(int i = 0; i<10; i++) {
	enemyWalkRight[i].x1 = 0.0;
	enemyWalkRight[i].x2 = 0.0;
	enemyWalkRight[i].y1 = 0.0;
	enemyWalkRight[i].y2 = 1.0;

	if(i == 0) {
	    enemyWalkRight[i].x1 = enemyWalkRight[i].x2;
	}
	else {
	    enemyWalkRight[i].x1 = enemyWalkRight[i-1].x2;
	}
	enemyWalkRight[i].x2 = Inc;
	Inc += .1;


    }



}

BasicEnemy::~BasicEnemy()
{
    //dtor
}

void BasicEnemy::update()
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
      //<< "    int facing = "<< this->facing << endl
      << "    int leftBoundary = "<< this->leftBoundary << endl
      << "    int rightBoundary = "<< this->rightBoundary << endl;
    return report.str();
}

void BasicEnemy::movement()
{
    prevPosition[0] = body.center[0];
    prevPosition[1] = body.center[1];
    if(state != DEATH) {
        switch(body.orientation) {
            case FACING_LEFT:
                body.center[0] += -0.5;
                break;
            case FACING_RIGHT:
                body.center[0] += 0.5;
                break;
            default:
                break;
        }
    }
    body.center[1] += velocity[1];
    if (velocity[1] > -7)
        velocity[1] += gravity;
}

void BasicEnemy::onCollision(GameObject * obj)
{
    if (obj->id == SPIKE) {
        //state = DEATH;
    }
    else { // obj->id == PLATFORM
        if (prevPosition[0]  < obj->body.center[0] - obj->body.width) {
            body.center[0] = obj->body.center[0] - obj->body.width - body.width;
            body.orientation = FACING_LEFT;
        }

        if (prevPosition[0]  > obj->body.center[0] + obj->body.width) {
            body.center[0] = obj->body.center[0] + obj->body.width + body.width;
            body.orientation = FACING_RIGHT;
        }

        if (body.center[1] < obj->body.center[1] &&
            body.center[0] + body.width > obj->body.center[0] - obj->body.width &&
            body.center[0] - body.width < obj->body.center[0] + obj->body.width)
        {
            body.center[1] = obj->body.center[1] - obj->body.height - body.height;
            velocity[1] = 0;
        }

        if (body.center[1] > obj->body.center[1] &&
            body.center[0] + body.width > obj->body.center[0] - obj->body.width &&
            body.center[0] - body.width < obj->body.center[0] + obj->body.width)
        {
            body.center[1] = obj->body.center[1] + obj->body.height + body.height;
            velocity[1] = 0;

        }
    }
}

void BasicEnemy::switchDirection() {
    if (body.orientation == FACING_LEFT) {
        body.orientation = FACING_RIGHT;
    } else if (body.orientation == FACING_RIGHT) {
        body.orientation = FACING_LEFT;
    }
}
