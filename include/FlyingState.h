#ifndef FLYING_H
#define FLYING_H

#include "GenericState.h"

class FlyingState : public GenericState
{
private:
    /* data */
public:
    FlyingState(
        int leds[3],
        Servo &servo, 
        LiquidCrystal_I2C &lcd, 
        int pin_echo, 
        int pin_trig, 
        NewPing &sonarUsed,
        int pirState
    );
    ~FlyingState();

    void enterState() override;
    GenericState* update() override;
    void exitState() override;
};

#endif