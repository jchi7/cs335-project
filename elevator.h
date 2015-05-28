#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "gameObject.h"

class Elevator : public GameObject
{
    public:
        int delay;

        Elevator(float centerX, float lowerY, float upperY, int texture, float vertVelMult);
        virtual ~Elevator();
        void setUpperLimit(float newLimit);
        void setLowerLimit(float newLimit);
        void setVertSpeed(float newSpeed);
        float getUpperLimit();
        float getLowerLimit();
        float getVertSpeed();
        void movement();
        string debugReport();
    protected:
    private:
        float vertSpeed;
        float lowerLimit, upperLimit;
};

#endif // ELEVATOR_H
