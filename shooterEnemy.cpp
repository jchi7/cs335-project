#include "shooterEnemy.h"

ShooterEnemy::ShooterEnemy(int left, int right, int x, int y)
{
    this->body.type = RECTANGLE;
    this->id = SHOOTERENEMY;
    this->body.width = 15;
    this->body.height = 17;
    this->body.center[0] = x;
    this->body.center[1] = y;
    this->body.center[2] = 0;
    this->prevPosition[0] = x;
    this->prevPosition[1] = y;
    this->prevPosition[2] = 0;
    this->velocity[0] = 0.5;
    this->velocity[1] = 0;
    this->velocity[2] = 0;
    this->rgb[0] = 0;
    this->rgb[1] = 0;
    this->rgb[2] = 0;
    this->body.orientation = FACING_RIGHT;
    state = WALKING;
    this->delay = 0;

    Inc = .1;
    for (int i =0; i < 10; i ++) {
        sprite[i].x1 = 0.0;
        sprite[i].x2 = 0.0;
        sprite[i].y1 = 0.0;
        sprite[i].y2 = 1.0;

        if (i == 0) {
            sprite[i].x1 = sprite[i].x2;
        }
        else {
            sprite [i].x1 = sprite[i - 1].x2;
        }
        sprite[i].x2 = Inc;
        Inc+= .1;
    }

}

ShooterEnemy::~ShooterEnemy()
{
    //dtor
}

void ShooterEnemy::update()
{
    //InputComponent->update(game, entity);
}

string ShooterEnemy::debugReport()
{
    ostringstream report;
    report << GameObject::debugReport()
      << "  ----"
      << "  SUBCLASS REPORT: ShooterEnemy\n"
      << "    Vec prevPosition = "<< vecPrint(this->prevPosition) << endl
      << "    Vec velocity = "<< vecPrint(this->velocity) << endl;
    return report.str();
}

void ShooterEnemy::movement()
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

void ShooterEnemy::onCollision(GameObject * obj)
{
    if (obj->id == SPIKE || obj->id == HBULLET) {
        state = DEATH;
    } else if (obj->id == EBULLET) {
        return;
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

void ShooterEnemy::switchDirection() {
    if (body.orientation == FACING_LEFT) {
        body.orientation = FACING_RIGHT;
    } else if (body.orientation == FACING_RIGHT) {
        body.orientation = FACING_LEFT;
    }
}
