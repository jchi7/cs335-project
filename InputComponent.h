#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H
#include "structs.h"
#include "gameObject.h"
#include "game.h"

class InputComponent
{
    public:
        InputComponent();
        virtual ~InputComponent();
        virtual void update(gameObject* entity, game* game);
    protected:
    private:
};

class genericInputComponent : public InputComponent
{
    public:
        genericInputComponent();
        virtual ~genericInputComponent();
        virtual void update();
    protected:
    private:
};

#endif // INPUTCOMPONENT_H
