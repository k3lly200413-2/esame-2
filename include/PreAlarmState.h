#ifndef PREAL_H
#define PREAL_H

#include "GenericState.h"

class PreAlarmState : public GenericState
{
private:
    /* data */
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
        float beta
    );
    ~PreAlarmState();

    void enterState() override;
    GenericState* update() override;
    void exitState() override;

};

#endif