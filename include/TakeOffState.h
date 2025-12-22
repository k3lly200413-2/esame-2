#ifndef TAKEOFF_H
#define TAKEOFF_H

#include "GenericState.h"
#include "StateManagerHeader.h"

class TakeOffState : public GenericState
{
private:
    unsigned long initialTime;
    unsigned long currentTime;
    int pirPin;
    int T1;
    int D1;
    unsigned long lastBlinkTime;
    bool isLedOn;
    
public:
    TakeOffState(
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
    
    ~TakeOffState();

    void enterState() override;
    bool canEmergencyStop() const override; 
    GenericState* update() override;
    void exitState() override;
    GenericState* clone() override;
};


#endif