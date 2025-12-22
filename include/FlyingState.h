#ifndef FLYING_H
#define FLYING_H

#include "GenericState.h"

class FlyingState : public GenericState
{
private:
    int pirOutPinUsed;
    bool sensorReset;
    int pirPin;
public:
    FlyingState(
        int leds[3],
        Servo &servo, 
        LiquidCrystal_I2C &lcd, 
        int pin_echo, 
        int pin_trig, 
        NewPing &sonarUsed,
        int pirOutPin,
        uint8_t analog_pin,
        float beta
    );
    ~FlyingState();

    void enterState() override;
    bool canEmergencyStop() const override; 
    GenericState* update() override;
    void exitState() override;
    GenericState* clone() override;
};

#endif