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
    
public:
    TakeOffState(
        int leds[3], Servo &servo, LiquidCrystal_I2C &lcd
    );
    
    ~TakeOffState();

    void enterState() override;
    void update() override;
    void exitState() override;

};


#endif