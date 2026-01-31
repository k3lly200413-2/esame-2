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
    // Store the previous state so we can resume operations later
    this->previousState = previousState;
}

FullAlarmState::~FullAlarmState()
{
}

void FullAlarmState::enterState()
{
    clearScreen();
    
    // Safety measures: shutdown motor and indicate Alarm status
    closeMotor();
    turnOffAllLeds();
    changeLed(2); // Turn on Red LED (Index 2)
    
    writeOnDisplay(0, 0, "ALARM");
    
    // Notify Python script: 'A' = Alarm Active
    Serial.println('A');
}

bool FullAlarmState::canEmergencyStop() const
{
    // Already in alarm/stopped state, so emergency stop is redundant or invalid
    return false;
}

GenericState *FullAlarmState::update()
{
    // Manual Reset: User must press the button to clear the alarm
    if (digitalRead(BUTTON_PIN) == HIGH)
    {
        setAlarmState(false);
        // Restore the state the system was in before the alarm triggered
        return this->previousState;
    }
    return NULL;
}

void FullAlarmState::exitState()
{
    // Notify Python script: 'N' = Normal / Alarm Cleared
    Serial.println('N');
}

GenericState* FullAlarmState::getPreviousState()
{
    // Utility to recover the stored state manually if needed
    setAlarmState(false);
    return this->previousState;
}

GenericState *FullAlarmState::clone()
{
    // Pass 'this' as the previous state to maintain history in the new clone
    return new FullAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta, this);
}