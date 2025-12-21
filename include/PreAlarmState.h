#ifndef PREAL_H
#define PREAL_H

#include "GenericState.h"

class PreAlarmState : public GenericState
{
private:
    int maxTemp2;
    unsigned int initalTime;
    unsigned int T4;
    GenericState* previousState;

public:
    PreAlarmState(
        int leds[3], 
        Servo &servo, 
        LiquidCrystal_I2C &lcd, 
        int pin_echo, 
        int pin_trig, 
        NewPing &sonarUsed,
        int pirState,
        uint8_t analog_pin,
        float beta,
        GenericState* previousState
    );
    ~PreAlarmState();

    void enterState() override;
    bool canEmergencyStop() const override; 
    GenericState* update() override;
    void exitState() override;

};

#endif