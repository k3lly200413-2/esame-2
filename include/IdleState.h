#ifndef IDLE_H
#define IDLE_H

#include "GenericState.h"


class IdleState : public GenericState
{
private:
    int pirPin;
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
        float beta
    );
    ~IdleState();

    void enterState() override;
    bool canEmergencyStop() const override; 
    GenericState* update() override;
    void exitState() override;
    GenericState* clone() override;
};

#endif