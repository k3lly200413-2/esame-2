#ifndef IDLE_H
#define IDLE_H

#include "GenericState.h"

#define T3 5
class IdleState : public GenericState
{
private:
    float maxTemp;
    int initalTime;
public:
    IdleState(
        int leds[3],
        Servo &servo, 
        LiquidCrystal_I2C &lcd, 
        int pin_echo, 
        int pin_trig, 
        NewPing &sonarUsed,
        int pirState,
        uint8_t analog_pin,
        int beta
    );
    ~IdleState();

    void enterState() override;
    GenericState* update() override;
    void exitState() override;
};

#endif