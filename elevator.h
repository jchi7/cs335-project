#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "gameObject.h"

class Elevator : public GameObject
{
    public:
        int delay;

        Elevator(float centerX, float lowerY, float upperY,int);
        virtual ~Elevator();
        void setUpperLimit(float newLimit);
        void setLowerLimit(float newLimit);
        float getUpperLimit();
        float getLowerLimit();
        void movement();
        string debugReport();
    protected:
    private:
        float lowerLimit, upperLimit;
};

#endif // ELEVATOR_H
