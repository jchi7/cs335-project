#include "firstBoss.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "spike.h"
#include "platform.h"
#include "fernandoV.h"
#include <math.h>

#define PI 3.14159265
#define GRAVITY -0.10

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
    this->sleepTimer = -200;
    this->attackCount = 0;
    this->alphaIncrease = true;
    this->alph = 0.0; 
    srand (time(NULL));
    this->numParticles = 0;
    this->increase = true;
    this->bossHitCounter = 100;
    this->spikeHitCounter = 50;
    this->beamPosition = 0;
    this->showBeam = false;
    this->unwindCircleCounter = 120;
    this->chasingSpikeIndex = 0;
    this->firstCircle = true;
    this->secondCircle = true;
    this->thirdCircle = true;
    this->bloodIndex = 0;
    this->particleVelocity = 1.0;
    this->bottomPlatformIndex = room->platforms.size();
    room->platforms.push_back(new Platform(480,30,500,30,0));
    room->numPlatforms++;
    this->rightPlatformIndex = room->numPlatforms;
    room->platforms.push_back(new Platform(15,15,1015,295,0));
    room->numPlatforms++;
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
    for (int i = numParticles-1; i >= 0; i--)
    {
        delete particles[i];
        particles.erase(particles.begin() + i);
    }
}
void FirstBoss::reset()
{
}
void FirstBoss::run(Hero * hero, Room * room)
{
    Shape * s;
    Shape * h = &hero->body;
    int count = 0;
    if (bloodIndex != 0 && bossAttack == FIFTH_ATTACK)
    {
        for (int i = bloodIndex; i < bloodIndex + 50; i++)
        {
            particles[i]->center[0] += bloodVelocity[count][0];
            bloodVelocity[count][1] += GRAVITY;
            particles[i]->center[1] += bloodVelocity[count][1];
            count++;
        }
    }
    if (bossAttack == FIFTH_ATTACK && sleepTimer > 100)
    {
        particleVelocity += GRAVITY;
        for (int i = 12; i < 140 ; i++)
        {
            particles[i]->center[1] += particleVelocity;
        }
    }
    for (int i = 0; i < numParticles; i++)
    {
        s = particles[i];
        if (s->center[0] < h->center[0]+h->width &&
                s->center[0] > h->center[0] - h->width &&
                s->center[1] < h->center[1] + h->height &&
                s->center[1] > h->center[1] - h->height)
        {
            hero->state = DEATH;
        }
    }
    if (bossAttack == FIFTH_ATTACK && bloodIndex == 0)
    {
        if (collisionRectRect(&room->spikes[chasingSpikeIndex]->body, &body))
        {
            killBoss(room);
            delete room->spikes[chasingSpikeIndex];
            room->spikes.erase(room->spikes.begin() + chasingSpikeIndex);
            room->numSpikes--;
        }
    }
    for (int i = room->numBullet-1; i >= 0; i--)
    {
        if (collisionRectRect(&room->spikes[chasingSpikeIndex]->body, &room->bullet[i]->body)) 
        {
            spikeHitCounter -=3;
            if (spikeHitCounter <= 0)
            {
                sleepTimer = 0;
                bossAttack = FIFTH_ATTACK;
                spikeHitCounter = 0;
            }
        }
        if (collisionRectRect(&body, &room->bullet[i]->body))
        {
            if (state != PREDEATH)
            {
                if (hero->body.center[0] < body.center[0])
                {
                    body.center[0] += 5;
                }
                else
                {
                    body.center[0] -= 5;
                }
                bossHitCounter-=3;
            }
            if (bossHitCounter == 1)
            {
                state=PREDEATH;
            }
            delete room->bullet[i];
            room->bullet.erase(room->bullet.begin() + i);
            room->numBullet--;
        }
    }
    if (collisionRectRect(&body, &hero->body))
        hero->state = DEATH; 
    switch (bossAttack)
    {
        case FIRST_ATTACK:
            firstAttack(hero, room);
            break;
        case SECOND_ATTACK:
            if (sleepTimer < 1500)
            {
                secondAttack(room);
            }
            else if (sleepTimer < 2300)
            {
                endSecondAttack(hero,room);
                sleepTimer ++;
            }
            else
            {
                for (int i = room->numSpikes-1; i > rightSpikeIndex + 6; i--)
                {
                    room->spikes.erase(room->spikes.begin() + i);
                    room->numSpikes--;
                }
                sleepTimer = 0;
                bossAttack = THIRD_ATTACK;
                createBulletCircle(800,600,100,30,12,false,0);
                room->platforms[bottomPlatformIndex]->body.center[1] = 30;
                if (startingPosition == WAIT)
                    startingPosition = RIGHT;

            }
            break;
        case THIRD_ATTACK:
            thirdAttack(room);
            break;
        case FOURTH_ATTACK:
            fourthAttack(hero,room);
            break;
        case FIFTH_ATTACK:
            if (bloodIndex == 0)
                fifthAttack(room);
            else
            {
                if (room->platforms[rightPlatformIndex]->body.center[0] > 965)
                    room->platforms[rightPlatformIndex]->body.center[0]--;
                else
                {
                    if (room->platforms[rightPlatformIndex]->body.center[1] > 160)
                        room->platforms[rightPlatformIndex]->body.center[1]--;
                }
                        
            }
            break;
        default:
            break;
    }
}
void FirstBoss::render(Room * room)
{
    glDisable(GL_TEXTURE_2D);
    //  glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (bloodIndex == 0)
    {
        glPushMatrix();
        glColor3f(0.0,0.0,0.9);
        glBegin(GL_QUADS);
        glVertex2i(body.center[0] - body.width, body.center[1] - body.height);
        glVertex2i(body.center[0] - body.width, body.center[1] + body.height);
        glVertex2i(body.center[0] + body.width, body.center[1] + body.height);
        glVertex2i(body.center[0] + body.width, body.center[1] - body.height);
        glEnd();
        glPopMatrix();
    }
    for (int i = 0; i < numParticles; i++)
    {
        glPushMatrix();
        glColor3f(1.0,0.0,1.0);
        glBegin(GL_QUADS);
        glVertex2i(particles[i]->center[0] - particles[i]->width, particles[i]->center[1] - particles[i]->height);
        glVertex2i(particles[i]->center[0] - particles[i]->width, particles[i]->center[1] + particles[i]->height);
        glVertex2i(particles[i]->center[0] + particles[i]->width, particles[i]->center[1] + particles[i]->height);
        glVertex2i(particles[i]->center[0] + particles[i]->width, particles[i]->center[1] - particles[i]->height);
        glEnd();
        glPopMatrix();
    }
    //    glDisable(GL_BLEND);
    if (alphaIncrease)
    {
        alph += 0.02;
        if (alph >= 0.90)
            alphaIncrease = false;
    }
    else
    {
        alph -= 0.02;
        if (alph <= 0)
            alphaIncrease = true;
    }
    if (showBeam && bossAttack == THIRD_ATTACK )
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
        glColor4f(1.0,0.0,0.0,alph);
        glBegin(GL_QUADS);
        glVertex2i(beamPosition - 13, 700);
        glVertex2i(beamPosition + 13, 700);
        glVertex2i(beamPosition + 13, 0);
        glVertex2i(beamPosition - 13, 0);
        glEnd();
        glPopMatrix();
        glDisable(GL_BLEND);
    }
    if (bloodIndex == 0)
    {
        glPushMatrix();
        glColor3f(0.0,0.8,0.0);
        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glVertex2i(body.center[0] - 50, body.center[1] + body.height + 10);
        glVertex2i(body.center[0] - 50 + bossHitCounter , body.center[1] + body.height + 10);
        glEnd();
        glPopMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
        glColor4f(0.0,0.8,0.0,0.3);
        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glVertex2i(body.center[0] - 50 + bossHitCounter , body.center[1] + body.height + 10);
        glVertex2i(body.center[0] - 50 + 100 , body.center[1] + body.height + 10);
        glEnd();
        glPopMatrix();
        glDisable(GL_BLEND);
    }
    if (chasingSpikeIndex && bloodIndex == 0)
    {
        glPushMatrix();
        glColor3f(0.0,0.8,0.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(room->spikes[chasingSpikeIndex]->body.center[0] - 25, room->spikes[chasingSpikeIndex]->body.center[1] + 20);
        glVertex2i(room->spikes[chasingSpikeIndex]->body.center[0] - 25 + spikeHitCounter, room->spikes[chasingSpikeIndex]->body.center[1] + 20);
        glEnd();
        glPopMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPushMatrix();
        glColor4f(0.0,0.8,0.0,0.3);
        glBegin(GL_LINE_LOOP);
        glVertex2i(room->spikes[chasingSpikeIndex]->body.center[0] - 25 + spikeHitCounter, room->spikes[chasingSpikeIndex]->body.center[1] + 20);
        glVertex2i(room->spikes[chasingSpikeIndex]->body.center[0] - 25 + 50, room->spikes[chasingSpikeIndex]->body.center[1] + 20);
        glEnd();
        glPopMatrix();
        glDisable(GL_BLEND);

    }
    if (state == PREDEATH && bloodIndex == 0)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(4);
        glPushMatrix();
        glColor4f(.2,.5,.8,alph);
        glBegin(GL_LINE_LOOP);
        glVertex2i(body.center[0] - body.width - 1, body.center[1] - body.height);
        glVertex2i(body.center[0] - body.width - 1, body.center[1] + body.height);
        glVertex2i(body.center[0] + body.width + 1, body.center[1] + body.height);
        glVertex2i(body.center[0] + body.width + 1, body.center[1] - body.height);
        glVertex2i(body.center[0] - body.width - 1, body.center[1] - body.height);
        glEnd();
        glPopMatrix();
        glDisable(GL_BLEND);
    }
    glLineWidth(1);
    glEnable(GL_TEXTURE_2D);


}
void FirstBoss::firstAttack(Hero * hero, Room * room)
{

    if (room->platforms[rightPlatformIndex]->body.center[0] > 995)
    {
        room->platforms[rightPlatformIndex]->body.center[0]--;
    }
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
                    playStomp();
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
                    playStomp();
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
                    playStomp();
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
                    playStomp();
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
                    playStomp();
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
                    playStomp();
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
                        if (attackCount == 8)
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
                        if (attackCount == 8)
                            destinationPosition = LEFT;
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
                room->platforms[bottomPlatformIndex]->body.center[1] += 8;
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
                    room->spikes[i]->body.corners[0][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] -= 23.0/50.0;
                }
                else
                {
                    room->spikes[i]->body.corners[0][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] += 23.0/50.0;
                }
            }
            for (int i = centerSpikeIndex; i < centerSpikeIndex + 7; i++)
            {
                if (sleepTimer <= 60)
                {
                    room->spikes[i]->body.corners[0][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] -= 23.0/50.0;
                }
                else
                {
                    room->spikes[i]->body.corners[0][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] += 23.0/50.0;
                }
            }

            break;
        case RIGHT:
            for (int i = leftSpikeIndex; i < leftSpikeIndex + 7; i++)
            {
                if (sleepTimer <= 60)
                {
                    room->spikes[i]->body.corners[0][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] -= 23.0/50.0;
                }
                else
                {
                    room->spikes[i]->body.corners[0][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] += 23.0/50.0;
                }
            }
            for (int i = centerSpikeIndex; i < centerSpikeIndex + 7; i++)
            {
                if (sleepTimer <= 60)
                {
                    room->spikes[i]->body.corners[0][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] -= 23.0/50.0;
                }
                else
                {
                    room->spikes[i]->body.corners[0][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] += 23.0/50.0;
                }
            }
            break;
        case CENTER:
            for (int i = leftSpikeIndex; i < leftSpikeIndex + 7; i++)
            {
                if (sleepTimer <= 60)
                {
                    room->spikes[i]->body.corners[0][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] -= 23.0/50.0;
                }
                else
                {
                    room->spikes[i]->body.corners[0][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] += 23.0/50.0;
                }
            }
            for (int i = rightSpikeIndex; i < rightSpikeIndex + 7; i++)
            {
                if (sleepTimer <= 60)
                {
                    room->spikes[i]->body.corners[0][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] -= 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] -= 23.0/50.0;
                }
                else
                {
                    room->spikes[i]->body.corners[0][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[1][1] += 23.0/50.0;
                    room->spikes[i]->body.corners[2][1] += 23.0/50.0;
                }
            }

            break;
        default:
            break;
    }
}
void FirstBoss::secondAttack(Room * room)
{
    if (sleepTimer % 40 == 0)
    {
        Vec spike[3];
        spike[0][0] = 1000; 
        spike[0][1] = (rand()%380) + 120; 
        spike[0][2] = 0; 
        spike[1][0] = 1000; 
        spike[1][1] = spike[0][1] + 26; 
        spike[1][2] = 0; 
        spike[2][0] = 977; 
        spike[2][1] = spike[0][1] + 13; 
        spike[2][2] = 0; 
        room->spikes.push_back(new Spike(spike,FACING_LEFT));
        room->numSpikes++;
        spike[0][0] = 0; 
        spike[0][1] = (rand()%380) + 120; 
        spike[0][2] = 0; 
        spike[1][0] = 0; 
        spike[1][1] = spike[0][1] + 26; 
        spike[1][2] = 0; 
        spike[2][0] = 23; 
        spike[2][1] = spike[0][1] + 13; 
        spike[2][2] = 0; 
        room->spikes.push_back(new Spike(spike,FACING_RIGHT));
        room->numSpikes++;
    }
    sleepTimer++;
    for (int i = rightSpikeIndex + 7; i < room->numSpikes; i++)
    {
        if (room->spikes[i]->body.orientation == FACING_LEFT)
        {
            room->spikes[i]->body.corners[0][0] -= 2;
            room->spikes[i]->body.corners[1][0] -= 2;
            room->spikes[i]->body.corners[2][0] -= 2;
            if (room->spikes[i]->body.corners[0][0] <= 0)
            {
                room->spikes.erase(room->spikes.begin() + i);
                room->numSpikes--;
            }
        }
        else
        {
            room->spikes[i]->body.corners[0][0] += 2;
            room->spikes[i]->body.corners[1][0] += 2;
            room->spikes[i]->body.corners[2][0] += 2;
            if (room->spikes[i]->body.corners[0][0] >= 1000)
            {
                room->spikes.erase(room->spikes.begin() + i);
                room->numSpikes--;
            }
        }
    }
}
void FirstBoss::endSecondAttack(Hero * hero, Room * room)
{
    for (int i = rightSpikeIndex + 7; i < room->numSpikes; i++)
    {
        if (sleepTimer < 1620)
        {
            if (room->spikes[i]->body.orientation == FACING_LEFT)
            {
                room->spikes[i]->body.corners[1][0] -= 26.0/120.0;
                room->spikes[i]->body.corners[1][1] -= 26.0/120.0;
                room->spikes[i]->body.corners[2][0] += 11.5/120.0;
                room->spikes[i]->body.corners[2][1] -= 36.0/120.0;
            }
            else
            {
                room->spikes[i]->body.corners[1][0] += 26.0/120.0;
                room->spikes[i]->body.corners[1][1] -= 26.0/120.0;
                room->spikes[i]->body.corners[2][0] -= 11.5/120.0;
                room->spikes[i]->body.corners[2][1] -= 36.0/120.0;
            }
        }
        else
        {
            std::cout << (sleepTimer - 1620) / 10 << std::endl;
            if ( ((sleepTimer - 1620) / 20) >= (i - (rightSpikeIndex + 7) ) )
            {
                if (hero->body.center[0] < room->spikes[i]->body.corners[2][0])
                {
                    room->spikes[i]->body.corners[0][0] -= 1;
                    room->spikes[i]->body.corners[1][0] -= 1;
                    room->spikes[i]->body.corners[2][0] -= 1;
                }
                else
                {
                    room->spikes[i]->body.corners[0][0] += 1;
                    room->spikes[i]->body.corners[1][0] += 1;
                    room->spikes[i]->body.corners[2][0] += 1;
                }
                room->spikes[i]->body.corners[0][1] -= 4;
                room->spikes[i]->body.corners[1][1] -= 4;
                room->spikes[i]->body.corners[2][1] -= 4;
            }
        }
    }
   
}
void FirstBoss::thirdAttack(Room * room)
{
    if (sleepTimer < 255)
    {
        createBulletCircle(body.center[0],body.center[1],0,5,12,true,0);
        body.center[1] -=2;
    }
    if (sleepTimer >250 && sleepTimer < 450)
    {
        if (increase)
        {
            createBulletCircle(body.center[0],body.center[1],2,0.5,12,true,0);
            if (circleIndexes[0][3] > 350)
                increase = false;
        }
        else
        {
            createBulletCircle(body.center[0],body.center[1],-2,-0.5,12,true,0);
            if (circleIndexes[0][3] < 100)
                increase = true;
        }
    }
    if (sleepTimer > 500 && sleepTimer < 850)
    {
        if (startingPosition == LEFT)
            createBulletCircle(body.center[0] + (sleepTimer - 500)*3, body.center[1],0,1,12,true,0);
        else
            createBulletCircle(body.center[0] - (sleepTimer - 500)*3, body.center[1],0,1,12,true,0);

    }
    if (sleepTimer > 850 && state != PREDEATH)
    {
        createBulletCircle(body.center[0], body.center[1],0,1,12,true,0);
        if (startingPosition == LEFT)
        {
            body.center[0] += 2;
            if (body.center[0] > 800)
                startingPosition = RIGHT;
        }
        if (startingPosition == RIGHT)
        {
            body.center[0] -= 2;
            if (body.center[0] < 200)
                startingPosition = LEFT;
        }
        if ((sleepTimer - 850) % 500 == 0)
        {
            Vec spike[3];
            spike[0][0] = beamPosition - 13;
            spike[0][1] = 60;
            spike[0][2] = 0;
            spike[1][0] = spike[0][0] + 26;
            spike[1][1] = 60;
            spike[1][2] = 0;
            spike[2][0] = spike[0][0] + 13;
            // (sqrt(3) / 2 ) * width  = height for equilateral triangle
            spike[2][1] = spike[0][1] + 22.981;
            spike[2][2] = 0;

            room->spikes.push_back(new Spike(spike,FACING_UP));
            room->numSpikes++;
            beamPosition = rand() % 1000;
            showBeam = true;
        }
    }
    if (state == PREDEATH)
    {
        if (unwindCircleCounter >= 0)
        {
            createBulletCircle(body.center[0],body.center[1],-1,0,unwindCircleCounter/10,true,0);
            unwindCircleCounter--;
        }
        else
        {
            bossAttack = FOURTH_ATTACK;
            sleepTimer = 0;
        }
    }
    sleepTimer++;

}
void FirstBoss::createBulletCircle(float centerx,float centery, float radius, float startingDegree, int numBullets,bool modify,int circleIndex)
{
    int startingIndex = 0;
    int endingIndex = 0;
    int count = 0;
    if (!modify)
    {
        std::vector<float> row;
        row.push_back(numParticles);
        row.push_back(numParticles+numBullets - 1);
        row.push_back(startingDegree);
        row.push_back(radius);
        circleIndexes.push_back(row);
        for (int i = 0; i < numBullets; i++)
        {
            particles.push_back(new Shape);
            numParticles++;
            particles[particles.size()-1]->center[0] = centerx + sin (startingDegree*(float)(i+1)*PI/180.0) * radius;
            particles[particles.size()-1]->center[1] = centery + cos (startingDegree*(float)(i+1)*PI/180.0) * radius;
            particles[particles.size()-1]->width = 3;
            particles[particles.size()-1]->height = 3;    
        }
    }
    else
    {
        startingIndex = circleIndexes[circleIndex][0];
        endingIndex = circleIndexes[circleIndex][1];
        circleIndexes[circleIndex][2] += startingDegree;
        circleIndexes[circleIndex][3] += radius;
        for (int i = startingIndex; i <= endingIndex; i++)
        {
            particles[i]->center[0] = centerx + sin ( (circleIndexes[circleIndex][2]+( (360.0/(float)numBullets)*(float)count)) * PI/180.0) * circleIndexes[circleIndex][3];
            particles[i]->center[1] = centery + cos ( (circleIndexes[circleIndex][2]+( (360.0/(float)numBullets)*(float)count)) * PI/180.0) * circleIndexes[circleIndex][3];
            count++;
        }
    }
}
void FirstBoss::fourthAttack(Hero * hero, Room * room)
{
    if (sleepTimer < 195)
    {
        body.center[1] += 2;
        body.center[0] += (500 - body.center[0]) / (195-sleepTimer);
    }
    if (sleepTimer == 195)
    {
        Vec spike[3];
        spike[0][0] = body.center[0] + 13;
        spike[0][1] = body.center[1] - body.height - 10;
        spike[0][2] = 0;
        spike[1][0] = spike[0][0] - 26;
        spike[1][1] = spike[0][1];
        spike[1][2] = 0;
        spike[2][0] = spike[0][0] - 13;
        // (sqrt(3) / 2 ) * width  = height for equilateral triangle
        spike[2][1] = spike[0][1] - 22.981;
        spike[2][2] = 0;

        room->spikes.push_back(new Spike(spike,FACING_DOWN));
        chasingSpikeIndex = room->numSpikes;
        room->numSpikes++;
        room->spikes[chasingSpikeIndex]->body.height = 15;
        room->spikes[chasingSpikeIndex]->body.width = 15;

    }
    if (sleepTimer > 195)
    {
        spikeChase(hero,room);
    }
    if (sleepTimer == 400)
    {
        createBulletCircle(body.center[0],body.center[1],100,360/32,32,false,1);
    //    createBulletCircle(body.center[0],body.center[1],-100,360/32,32,false,2);
    //    createBulletCircle(body.center[0],body.center[1],-200,360/32,32,false,1);
    //    createBulletCircle(body.center[0],body.center[1],0,360/32,32,false,3);
    }
    if (sleepTimer > 400)
    {
        if (!firstCircle)
            createBulletCircle(body.center[0],body.center[1],0.5,0.25,32,true,2);
        if (!secondCircle)
            createBulletCircle(body.center[0],body.center[1],0.5,0.25,32,true,3);
        if (!thirdCircle)
            createBulletCircle(body.center[0],body.center[1],0.5,0.25,32,true,4);
        createBulletCircle(body.center[0],body.center[1],0.5,0.25,32,true,1);
        if (sleepTimer == 600)
        {
            if (firstCircle == false)
                createBulletCircle(body.center[0],body.center[1],-400,0.5,32,true,2);
            else
                createBulletCircle(body.center[0],body.center[1],100,360/32,32,false,2);
            firstCircle = false;

        }
        if (sleepTimer == 800)
        {
            if (secondCircle == false)
                createBulletCircle(body.center[0],body.center[1],-400,0.5,32,true,3);
            else
                createBulletCircle(body.center[0],body.center[1],100,360/32,32,false,3);
            secondCircle = false;
        }
        if (sleepTimer == 1000)
        {
            if (thirdCircle == false)
                createBulletCircle(body.center[0],body.center[1],-400,0.5,32,true,4);
            else
                createBulletCircle(body.center[0],body.center[1],100,360/32,32,false,4);
            thirdCircle = false;

        }
        if (sleepTimer == 1200)
        {
            createBulletCircle(body.center[0],body.center[1],-400,0,32,true,1);
            sleepTimer = 401;
        }
    }
    sleepTimer++;
}
void FirstBoss::spikeChase(GameObject * hero, Room * room)
{
    Spike * spike = (Spike*)room->spikes[chasingSpikeIndex];
    float x = hero->body.center[0] - spike->body.corners[2][0];
    float y = hero->body.center[1] - spike->body.corners[2][1];
    float z = sqrt(pow(x,2) + pow(y,2));
    float angle = atan(fabs(x/y));
    z -= 1;
    float newX = sin(angle) * z;
    float newY = cos(angle) * z;
    if (x > 0)
    {
        spike->body.corners[2][0] += x - newX;
        spike->body.corners[1][0] += x - newX;
        spike->body.corners[0][0] += x - newX;
    }
    else
    {
        spike->body.corners[2][0] += x + newX;
        spike->body.corners[1][0] += x + newX;
        spike->body.corners[0][0] += x + newX;
    }
    if (y > 0)
    {
        spike->body.corners[2][1] += y - newY;
        spike->body.corners[1][1] += y - newY;
        spike->body.corners[0][1] += y - newY;
    }
    else
    {
        spike->body.corners[2][1] += y + newY;
        spike->body.corners[1][1] += y + newY;
        spike->body.corners[0][1] += y + newY;
    }

    switch (spike->body.orientation)
    {
        case FACING_UP:
            spike->body.corners[0][0] += (13.0 + (23.981/2.0));
            spike->body.corners[0][1] -= (13.0 - (23.981/2.0));
            spike->body.corners[1][0] = spike->body.corners[0][0];
            spike->body.corners[1][1] = spike->body.corners[0][1] + 26;
            spike->body.corners[2][0] = spike->body.corners[0][0] - 23.981;
            spike->body.corners[2][1] = spike->body.corners[0][1] + 13;
            spike->body.orientation = FACING_LEFT;
            spike->body.center[0] = spike->body.corners[2][0] + (23.981/2.0);
            spike->body.center[1] = spike->body.corners[2][1];
            break;
        case FACING_LEFT:
            spike->body.corners[0][0] += (13.0 - (23.981/2.0));
            spike->body.corners[0][1] += (13.0 + (23.981/2.0));
            spike->body.corners[1][0] = spike->body.corners[0][0] - 26;
            spike->body.corners[1][1] = spike->body.corners[0][1];
            spike->body.corners[2][0] = spike->body.corners[0][0] - 13;
            spike->body.corners[2][1] = spike->body.corners[0][1] - 23.981;
            spike->body.orientation = FACING_DOWN;
            spike->body.center[0] = spike->body.corners[2][0];
            spike->body.center[1] = spike->body.corners[2][1] + (23.981/2.0);
            break;
        case FACING_DOWN:
            spike->body.corners[0][0] -= (13.0 + (23.981/2.0));
            spike->body.corners[0][1] += (13.0 - (23.981/2.0));
            spike->body.corners[1][0] = spike->body.corners[0][0];
            spike->body.corners[1][1] = spike->body.corners[0][1] - 26;
            spike->body.corners[2][0] = spike->body.corners[0][0] + 23.981;
            spike->body.corners[2][1] = spike->body.corners[0][1] - 13;
            spike->body.orientation = FACING_RIGHT;
            spike->body.center[0] = spike->body.corners[2][0] - (23.981/2.0);
            spike->body.center[1] = spike->body.corners[2][1];
            break;
        case FACING_RIGHT:
            spike->body.corners[0][0] -= (13.0 - (23.981/2.0));
            spike->body.corners[0][1] -= (13.0 + (23.981/2.0));
            spike->body.corners[1][0] = spike->body.corners[0][0] + 26;
            spike->body.corners[1][1] = spike->body.corners[0][1];
            spike->body.corners[2][0] = spike->body.corners[0][0] + 13;
            spike->body.corners[2][1] = spike->body.corners[0][1] + 23.981;
            spike->body.orientation = FACING_UP;
            spike->body.center[0] = spike->body.corners[2][0];
            spike->body.center[1] = spike->body.corners[2][1] - (23.981/2.0);
            break;
        default:
            break;
    }
    // spike->corners[2][0] += (hero->body.center[0] - spike->corners[2][0]) / 500;
    // spike->corners[2][1] += (hero->body.center[1] - spike->corners[2][1]) / 500;
}            
void FirstBoss::fifthAttack(Room * room)
{
    if (sleepTimer > 100)
    {
        if (bloodIndex > 0)
        {

        }
        spikeChase(this,room);
    }

    sleepTimer++;
}
void FirstBoss::killBoss(Room * room)
{
    for (int i = 0; i < 50; i++)
    {
        bloodVelocity[i][0] = (rand() % 10) - 5;
        bloodVelocity[i][1] = (rand() % 10) - 5;
        particles.push_back(new Shape);
        if (i == 0)
            bloodIndex = numParticles;
        numParticles++;
        particles[numParticles-1]->center[0] = body.center[0];
        particles[numParticles-1]->center[1] = body.center[1];
        particles[numParticles-1]->width = 3;
        particles[numParticles-1]->height = 3;
    }
}


