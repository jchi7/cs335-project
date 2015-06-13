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
        int bottomPlatformIndex;
        int rightPlatformIndex;
        int attackCount;
        bool alphaIncrease;
        float alph;
        int numParticles;
        bool increase;
        int bossHitCounter;
        int beamPosition;
        bool showBeam;
        int unwindCircleCounter;
        int chasingSpikeIndex;
        int spikeHitCounter;
        bool firstCircle;
        int bloodIndex;
        float particleVelocity;
        float bloodVelocity[50][2];
        std::vector<Shape *> particles;
        std::vector< std::vector<float> > circleIndexes;
        FirstBoss(){}
        FirstBoss(Room *);
        ~FirstBoss();
        void run(Hero *, Room *);
        void render(Room *);
        void firstAttack(Hero *, Room *);
        void secondAttack(Room *);
        void thirdAttack(Room *);
        void endSecondAttack(Room *);
        void ceilingSpikes(Room *, FirstAttackPosition, int);
        void createBulletCircle(float, float, float , float, int, bool,int);
        void fourthAttack(Hero*, Room*);
        void spikeChase(GameObject*, Room*);
        void fifthAttack(Room*);
        void killBoss(Room *);
        void reset();
    protected:
    private:
};

#endif // FIRSTBOSS_H
