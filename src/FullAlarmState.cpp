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
    float beta,
    GenericState* previousState
)
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState, analog_pin, beta)
{
    this->previousState = previousState;
}

FullAlarmState::~FullAlarmState()
{
}

void FullAlarmState::enterState()
{
    clearScreen();
    closeMotor();
    turnOffAllLeds();
    changeLed(2);
    writeOnDisplay(0, 0, "YO WE GONE DIE");
}

bool FullAlarmState::canEmergencyStop() const
{
    return false;
}

GenericState *FullAlarmState::update()
{
    return NULL;
}

void FullAlarmState::exitState()
{
}

GenericState* FullAlarmState::getPreviousState()
{
    return this->previousState;
}

GenericState *FullAlarmState::clone()
{
    return new FullAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta, this);
}
