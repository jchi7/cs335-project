#ifndef FIRSTBOSS_H
#define FIRSTBOSS_H

#include "gameObject.h"
#include "structs.h"
#include "hero.h"
#include "room.h"

enum FirstAttackPosition{LEFT,CENTER,RIGHT,WAIT};

class FirstBoss : public GameObject
{
    public:
        CharacterState state;
        Vec prevPosition;
        Vec velocity;
        AttackType bossAttack;
        FirstAttackPosition startingPosition;
        FirstAttackPosition destinationPosition;
        int sleepTimer;
        int nextPosition;
        bool goingUp;
        int leftSpikeIndex;
        int centerSpikeIndex;
        int rightSpikeIndex;
        int attackCount;
        FirstBoss(){}
        FirstBoss(Room *);
        ~FirstBoss();
        void run(Hero *, Room *);
        void render();
        void firstAttack(Hero *, Room *);
        void ceilingSpikes(Room *, FirstAttackPosition, int);
        void reset();
    protected:
    private:
};

#endif // FIRSTBOSS_H
