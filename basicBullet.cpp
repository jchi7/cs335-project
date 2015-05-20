#include "basicBullet.h"

BasicBullet::BasicBullet(int xVel, int yVel, int x, int y, ObjectType oid)
{
    this->body.type = RECTANGLE;
    //this->body.width = 4;
    //this->body.height = 4;
    this->body.width = 4;
    this->body.height = 4;
    this->body.center[0] = x;
    this->body.center[1] = y;
    this->body.center[2] = 0;
    this->prevPosition[0] = x;
    this->prevPosition[1] = y;
    this->prevPosition[2] = 0;
    this->velocity[0] = xVel;
    this->velocity[1] = yVel;
    this->velocity[2] = 0;
    this->rgb[0] = 0;
    this->rgb[1] = 0;
    this->rgb[2] = 0;
    this->body.orientation = FACING_RIGHT;
    state = WALKING;
    if (oid == HERO)
        this->id = HBULLET;
    else
        this->id = EBULLET;
    Inc = .1;
    for(int i = 0; i < 10; i++) {
        bullet[i].x1 = 0.0;
        bullet[i].x2 = 0.0;
        bullet[i].y1 = 0.0;
        bullet[i].y2 = 1.0;

        if (i == 0) {
            bullet[i].x1 = bullet[i-1].x2;
        }
        else {
            bullet[i].x1 = bullet[i-1].x2;
        }
        bullet[i].x2 = Inc;
        Inc += .1;
    }
}

BasicBullet::~BasicBullet()
{
    //dtor
}

void BasicBullet::update()
{
    //InputComponent->update(game, entity);
}

string BasicBullet::debugReport()
{
    ostringstream report;
    report << GameObject::debugReport()
      << "  ----"
      << "  SUBCLASS REPORT: BasicBullet\n"
      << "    Vec prevPosition = "<< vecPrint(this->prevPosition) << endl
      << "    Vec velocity = "<< vecPrint(this->velocity) << endl;
    return report.str();
}

void BasicBullet::movement()
{
    prevPosition[0] = body.center[0];
    prevPosition[1] = body.center[1];
    if(state != DEATH) {
        switch(body.orientation) {
            case FACING_LEFT:
                body.center[0] -= velocity[0];
                body.center[1] -= velocity[1];
                break;
            case FACING_RIGHT:
                body.center[0] += velocity[0];
                body.center[1] -= velocity[1];
                break;
            default:
                break;
        }
    }
    body.center[1] += velocity[1];
    if (velocity[1] > -7)
        velocity[1] += gravity;
    if (body.center[0] > 1000 || body.center[0] < 0 )
        state = DEATH;
}

void BasicBullet::onCollision(GameObject * obj)
{
    if (obj->id == SPIKE) {
        state = DEATH;
    } else if (obj->id == HBULLET || obj->id == EBULLET) {
        return;
    }
    else { // obj->id == PLATFORM
        if (prevPosition[0]  < obj->body.center[0] - obj->body.width) {
            body.center[0] = obj->body.center[0] - obj->body.width - body.width;
            state = DEATH;
        }

        if (prevPosition[0]  > obj->body.center[0] + obj->body.width) {
            body.center[0] = obj->body.center[0] + obj->body.width + body.width;
            state = DEATH;
        }

        if (body.center[1] < obj->body.center[1] &&
            body.center[0] + body.width > obj->body.center[0] - obj->body.width &&
            body.center[0] - body.width < obj->body.center[0] + obj->body.width)
        {
            body.center[1] = obj->body.center[1] - obj->body.height - body.height;
            state = DEATH;
        }

        if (body.center[1] > obj->body.center[1] &&
            body.center[0] + body.width > obj->body.center[0] - obj->body.width &&
            body.center[0] - body.width < obj->body.center[0] + obj->body.width)
        {
            body.center[1] = obj->body.center[1] + obj->body.height + body.height;
            state = DEATH;
        }
    }
}
