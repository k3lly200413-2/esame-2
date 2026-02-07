#include "FullAlarmState.h"
#include "ButtonsHeader.h"

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
    // Store the previous state so we can return to it later
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
    changeLed(2); // Turn on Red LED
    
    writeOnDisplay(0, 0, "ALARM");
    
    // Notify Python script: 'A'larm
    Serial.println('A');
}

bool FullAlarmState::canEmergencyStop() const
{
    // Already in alarm/stopped state, so emergency stop is not applicable 
    return false;
}

GenericState *FullAlarmState::update()
{
    // Manual Reset
    if (digitalRead(BUTTON_PIN) == HIGH)
    {
        setAlarmState(false);
        // Restore the previous state
        return this->previousState;
    }
    // otherwise recheck
    return NULL;
}

void FullAlarmState::exitState()
{
    // We are in 'N'ormal State
    Serial.println('N');
}

GenericState* FullAlarmState::getPreviousState()
{
    // get previous state
    setAlarmState(false);
    return this->previousState;
}

GenericState *FullAlarmState::clone()
{
    // pass this to allow previous state to work correctly
    return new FullAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta, this);
}