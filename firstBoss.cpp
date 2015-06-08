#include "firstBoss.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "spike.h"

FirstBoss::FirstBoss(Room * room)
{
    this->body.type = RECTANGLE;
    //this->id = ENEMY;
    this->body.width = 20;
    this->body.height = 30;
    this->body.center[0] = 800;
    this->body.center[1] = 600;
    this->body.center[2] = 0;
    this->prevPosition[0] = 900;
    this->prevPosition[1] = 100;
    this->prevPosition[2] = 0;
    this->velocity[0] = 0;
    this->velocity[1] = 0;
    this->velocity[2] = 0;
    this->rgb[0] = 0;
    this->rgb[1] = 0;
    this->rgb[2] = 0;
    // this->body.orientation = FACING_RIGHT;
    bossAttack = FIRST_ATTACK;
    this->startingPosition = WAIT;
    this->destinationPosition = WAIT;
    this->nextPosition = 0;
    this->goingUp = true;
    this->sleepTimer = 0;
    this->attackCount = 0;
    srand (time(NULL));
    Vec spike[3];
    for (int i = 0; i < 37; i++)
    {
        spike[0][0] = 20 + i*26;
        spike[0][1] = 700;
        spike[0][2] = 0;
        spike[1][0] = spike[0][0] + 26;
        spike[1][1] = 700;
        spike[1][2] = 0;
        spike[2][0] = spike[0][0] + 13;
        // (sqrt(3) / 2 ) * width  = height for equilateral triangle
        spike[2][1] = spike[0][1] - 22.981;
        spike[2][2] = 0;

        room->spikes.push_back(new Spike(spike,FACING_DOWN));
        room->numSpikes++;
    }
    leftSpikeIndex =  room->spikes.size();
    for (int i = 0; i < 7; i++)
    {
        spike[0][0] = 112 + i*25;
        spike[0][1] = 565;
        spike[0][2] = 0;
        spike[1][0] = spike[0][0] + 25;
        spike[1][1] = 565;
        spike[1][2] = 0;
        spike[2][0] = spike[0][0] + 12.5;
        // (sqrt(3) / 2 ) * width  = height for equilateral triangle
        spike[2][1] = spike[0][1] - 22.981;
        spike[2][2] = 0;

        room->spikes.push_back(new Spike(spike,FACING_DOWN));
        room->numSpikes++;
    }
    centerSpikeIndex =  room->spikes.size();
    for (int i = 0; i < 7; i++)
    {
        spike[0][0] = 412 + i*25;
        spike[0][1] = 565;
        spike[0][2] = 0;
        spike[1][0] = spike[0][0] + 25;
        spike[1][1] = 565;
        spike[1][2] = 0;
        spike[2][0] = spike[0][0] + 12.5;
        // (sqrt(3) / 2 ) * width  = height for equilateral triangle
        spike[2][1] = spike[0][1] - 22.981;
        spike[2][2] = 0;

        room->spikes.push_back(new Spike(spike,FACING_DOWN));
        room->numSpikes++;
    }
    rightSpikeIndex =  room->spikes.size();
    for (int i = 0; i < 7; i++)
    {
        spike[0][0] = 712 + i*25;
        spike[0][1] = 565;
        spike[0][2] = 0;
        spike[1][0] = spike[0][0] + 25;
        spike[1][1] = 565;
        spike[1][2] = 0;
        spike[2][0] = spike[0][0] + 12.5;
        // (sqrt(3) / 2 ) * width  = height for equilateral triangle
        spike[2][1] = spike[0][1] - 22.981;
        spike[2][2] = 0;

        room->spikes.push_back(new Spike(spike,FACING_DOWN));
        room->numSpikes++;
    }


}
FirstBoss::~FirstBoss()
{
}
void FirstBoss::reset()
{
}
void FirstBoss::run(Hero * hero, Room * room)
{
    switch (bossAttack)
    {
        case FIRST_ATTACK:
            firstAttack(hero, room);
            break;
        case SECOND_ATTACK:
            break;
        default:
            break;
    }
}
void FirstBoss::render()
{
    glDisable(GL_TEXTURE_2D);
    //  glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
    glColor3f(0.0,0.0,0.9);
    glBegin(GL_QUADS);
    glVertex2i(body.center[0] - body.width, body.center[1] - body.height);
    glVertex2i(body.center[0] - body.width, body.center[1] + body.height);
    glVertex2i(body.center[0] + body.width, body.center[1] + body.height);
    glVertex2i(body.center[0] + body.width, body.center[1] - body.height);
    glEnd();
    glPopMatrix();
    //    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
void FirstBoss::firstAttack(Hero * hero, Room * room)
{

    switch (startingPosition)
    {
        case RIGHT:
            if (destinationPosition == CENTER)
            {
                if (body.center[1] < 650 && goingUp)
                {
                    body.center[1] += 1;
                }
                else
                {
                    goingUp = false;
                    body.center[1] -= 1;
                }
                body.center[0] -= 3;
                if (body.center[1] == 600)
                {
                    startingPosition = WAIT;
                    if (hero->state != DEATH)
                    {
                        hero->velocity[1] = 20;
                        hero->jumpCount = 2;
                        hero->state = JUMPING;
                    }
                    body.center[0] = 500;
                    goingUp = true;
                }
            }
            else
            {
                if (body.center[1] < 650 && goingUp)
                {
                    body.center[1] += 1;
                }
                else
                {
                    goingUp = false;
                    body.center[1] -= 1;
                }
                body.center[0] -= 6;
                if (body.center[1] == 600)
                {
                    startingPosition = WAIT;
                    if (hero->state != DEATH)
                    {
                        hero->velocity[1] = 20;
                        hero->jumpCount = 2;
                        hero->state = JUMPING;
                    }
                    body.center[0] = 200;
                    goingUp = true;
                }
            }
            break;
        case LEFT:
            if (destinationPosition == CENTER)
            {
                if (body.center[1] < 650 && goingUp)
                {
                    body.center[1] += 1;
                }
                else
                {
                    goingUp = false;
                    body.center[1] -= 1;
                }
                body.center[0] += 3;
                if (body.center[1] == 600)
                {
                    startingPosition = WAIT;
                    if (hero->state != DEATH)
                    {
                        hero->velocity[1] = 20;
                        hero->jumpCount = 2;
                        hero->state = JUMPING;
                    }
                    body.center[0] = 500;
                    goingUp = true;
                }
            }
            else
            {
                if (body.center[1] < 650 && goingUp)
                {
                    body.center[1] += 1;
                }
                else
                {
                    goingUp = false;
                    body.center[1] -= 1;
                }
                body.center[0] += 6;
                if (body.center[1] == 600)
                {
                    startingPosition = WAIT;
                    if (hero->state != DEATH)
                    {
                        hero->velocity[1] = 20;
                        hero->jumpCount = 2;
                        hero->state = JUMPING;
                    }
                    body.center[0] = 800;
                    goingUp = true;
                }
            }
            break;
        case CENTER:
            if (destinationPosition == LEFT)
            {
                if (body.center[1] < 650 && goingUp)
                {
                    body.center[1] += 1;
                }
                else
                {
                    goingUp = false;
                    body.center[1] -= 1;
                }
                body.center[0] -= 3;
                if (body.center[1] == 600)
                {
                    startingPosition = WAIT;
                    if (hero->state != DEATH)
                    {
                        hero->velocity[1] = 20;
                        hero->jumpCount = 2;
                        hero->state = JUMPING;
                    }
                    body.center[0] = 200;
                    goingUp = true;
                }
            }
            else
            {
                if (body.center[1] < 650 && goingUp)
                {
                    body.center[1] += 1;
                }
                else
                {
                    goingUp = false;
                    body.center[1] -= 1;
                }
                body.center[0] += 3;
                if (body.center[1] == 600)
                {
                    startingPosition = WAIT;
                    if (hero->state != DEATH)
                    {
                        hero->velocity[1] = 20;
                        hero->jumpCount = 2;
                        hero->state = JUMPING;
                    }
                    body.center[0] = 800;
                    goingUp = true;
                }
            }
            break;
        case WAIT:
            sleepTimer++;
            ceilingSpikes(room,destinationPosition,sleepTimer);
            if (sleepTimer >= 120)
            {
                nextPosition = (int) (rand() % 2);
                switch ((int)body.center[0])
                {
                    case 200:
                        startingPosition = LEFT;
                        if ( nextPosition == 0)
                            destinationPosition = CENTER;
                        else
                            destinationPosition = RIGHT;
                        break;
                    case 500:
                        startingPosition = CENTER;
                        if (nextPosition == 0)
                            destinationPosition = LEFT;
                        else
                            destinationPosition = RIGHT;
                        break;
                    case 800:
                        startingPosition = RIGHT;
                        if (nextPosition == 0)
                            destinationPosition = LEFT;
                        else
                            destinationPosition = CENTER;
                        break;
                    default:
                        break;
                }
                sleepTimer = 0;
                attackCount++;
                if (attackCount >= 10)
                {
                    attackCount = 0;
                    bossAttack = SECOND_ATTACK;
                }
            }
            break;
        default:
            break;
    }
}
void FirstBoss::ceilingSpikes(Room * room, FirstAttackPosition position, int sleepTimer)
{
    Vec spike[3];
    switch (position)
    {
        case LEFT:
            for (int i = rightSpikeIndex; i < rightSpikeIndex + 7; i++)
            {
                if (sleepTimer <= 60)
                {
                room->spikes[i]->body.corners[0][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[1][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[2][1] -= 23.0/60.0;
                }
                else
                {
                room->spikes[i]->body.corners[0][1] += 23.0/60.0;
                room->spikes[i]->body.corners[1][1] += 23.0/60.0;
                room->spikes[i]->body.corners[2][1] += 23.0/60.0;
                }
            }
            for (int i = centerSpikeIndex; i < centerSpikeIndex + 7; i++)
            {
                if (sleepTimer <= 60)
                {
                room->spikes[i]->body.corners[0][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[1][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[2][1] -= 23.0/60.0;
                }
                else
                {
                room->spikes[i]->body.corners[0][1] += 23.0/60.0;
                room->spikes[i]->body.corners[1][1] += 23.0/60.0;
                room->spikes[i]->body.corners[2][1] += 23.0/60.0;
                }
            }

            break;
        case RIGHT:
            for (int i = leftSpikeIndex; i < leftSpikeIndex + 7; i++)
            {
                if (sleepTimer <= 60)
                {
                room->spikes[i]->body.corners[0][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[1][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[2][1] -= 23.0/60.0;
                }
                else
                {
                room->spikes[i]->body.corners[0][1] += 23.0/60.0;
                room->spikes[i]->body.corners[1][1] += 23.0/60.0;
                room->spikes[i]->body.corners[2][1] += 23.0/60.0;
                }
            }
            for (int i = centerSpikeIndex; i < centerSpikeIndex + 7; i++)
            {
                if (sleepTimer <= 60)
                {
                room->spikes[i]->body.corners[0][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[1][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[2][1] -= 23.0/60.0;
                }
                else
                {
                room->spikes[i]->body.corners[0][1] += 23.0/60.0;
                room->spikes[i]->body.corners[1][1] += 23.0/60.0;
                room->spikes[i]->body.corners[2][1] += 23.0/60.0;
                }
            }
            break;
        case CENTER:
            for (int i = leftSpikeIndex; i < leftSpikeIndex + 7; i++)
            {
                if (sleepTimer <= 60)
                {
                room->spikes[i]->body.corners[0][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[1][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[2][1] -= 23.0/60.0;
                }
                else
                {
                room->spikes[i]->body.corners[0][1] += 23.0/60.0;
                room->spikes[i]->body.corners[1][1] += 23.0/60.0;
                room->spikes[i]->body.corners[2][1] += 23.0/60.0;
                }
            }
            for (int i = rightSpikeIndex; i < rightSpikeIndex + 7; i++)
            {
                if (sleepTimer <= 60)
                {
                room->spikes[i]->body.corners[0][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[1][1] -= 23.0/60.0;
                room->spikes[i]->body.corners[2][1] -= 23.0/60.0;
                }
                else
                {
                room->spikes[i]->body.corners[0][1] += 23.0/60.0;
                room->spikes[i]->body.corners[1][1] += 23.0/60.0;
                room->spikes[i]->body.corners[2][1] += 23.0/60.0;
                }
            }

            break;
        default:
            break;
    }
} 
