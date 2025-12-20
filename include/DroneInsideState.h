#ifndef DRONE_INSIDE_H
#define DRONE_INSIDE_H

#include "GenericState.h"
#include "StateManagerHeader.h"



class DroneInsideState : public GenericState
{
    public:
    DroneInsideState(
        int leds[3], 
        Servo &servo, 
        LiquidCrystal_I2C &lcd, 
        int pin_echo, 
        int pin_trig, 
        NewPing &sonarUsed,
        int pirState
    );

    ~DroneInsideState();
    void enterState() override;
    GenericState* update() override;
    void exitState() override;
};

#endif