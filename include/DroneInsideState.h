#ifndef DRONE_INSIDE_H
#define DRONE_INSIDE_H

#include "GenericState.h"
#include "StateManagerHeader.h"



class DroneInsideState : public GenericState
{
    public:
    DroneInsideState(int leds[3], Servo &servo, LiquidCrystal_I2C &lcd, int pin_echo, int pin_trig);

    ~DroneInsideState();
    void enterState() override;
    void update() override;
    void exitState() override;
};

#endif