#ifndef TAKEOFF_H
#define TAKEOFF_H

#include "GenericState.h"
#include "StateManagerHeader.h"

class TakeOffState : public GenericState
{
private:
    unsigned long initialTime;
    unsigned long currentTime;
    int T1;
    int D1;
    int lastBlinkTime;
    bool isLedOn;
    
public:
    TakeOffState(
        int leds[3], 
        Servo &servo, 
        LiquidCrystal_I2C &lcd, 
        int pin_echo, 
        int pin_trig, 
        NewPing &sonarUsed,
        int pirState
    );
    
    ~TakeOffState();

    void enterState() override;
    GenericState* update() override;
    void exitState() override;

};


#endif