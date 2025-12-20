#include "FlyingState.h"

FlyingState::FlyingState(
    int leds[3],
    Servo &servo, 
    LiquidCrystal_I2C &lcd, 
    int pin_echo, 
    int pin_trig, 
    NewPing &sonarUsed,
    int pirState,
    uint8_t analog_pin,
    float beta
)
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState, analog_pin, beta)
{
}

FlyingState::~FlyingState()
{
}

void FlyingState::enterState()
{
    Serial.println("Entered Idle State");
}

GenericState* FlyingState::update()
{
    Serial.println("Updating Idle State. Doing Nothing");
    return NULL;
}

void FlyingState::exitState()
{
    Serial.println("Exited Idle State");
}
