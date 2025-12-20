#include "IdleState.h"

IdleState::IdleState(
    int leds[3],
    Servo &servo, 
    LiquidCrystal_I2C &lcd, 
    int pin_echo, 
    int pin_trig, 
    NewPing &sonarUsed,
    int pirState
)
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState)
{
}

IdleState::~IdleState()
{
}

void IdleState::enterState()
{
    Serial.println("Entered Idle State");
}

GenericState* IdleState::update()
{
    Serial.println("Updating Idle State. Doing Nothing");
    return NULL;
}

void IdleState::exitState()
{
    Serial.println("Exited Idle State");
}
