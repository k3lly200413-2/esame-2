#include "FullAlarmState.h"

FullAlarmState::FullAlarmState(
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

FullAlarmState::~FullAlarmState()
{
}

void FullAlarmState::enterState()
{
    clearScreen();
    closeMotor();
    turnOffAllLeds();
    changeLed(3);
    writeOnDisplay(0, 0, "YO WE GONE DIE");
}

GenericState *FullAlarmState::update()
{
    return NULL;
}

void FullAlarmState::exitState()
{
}
