#ifndef LANDING_H
#define LANDING_H

#include "GenericState.h"

class LandingState : public GenericState
{
private:
    unsigned long D2;
    unsigned long T2;
    unsigned long initialTime;
    unsigned long currentTime;
    int pirOutPinUsed;
    unsigned long lastBlinkTime;
    bool isLedOn;
    
public:
    LandingState(
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
    ~LandingState();

    void enterState() override;
    GenericState* update() override;
    void exitState() override;
};


#endif