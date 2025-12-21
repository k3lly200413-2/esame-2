#ifndef FULLALARM_H
#define FULLALARM_H

#include "GenericState.h"

class FullAlarmState : public GenericState
{
private:

public:
    FullAlarmState(
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
    ~FullAlarmState();

    void enterState() override;
    bool canEmergencyStop() const override; 
    GenericState* update() override;
    void exitState() override;

};

#endif