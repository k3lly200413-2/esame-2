#include "IdleState.h"
#include "PreAlarmState.h"
#include "TakeOffState.h"

IdleState::IdleState(
    int leds[3],
    Servo &servo, 
    LiquidCrystal_I2C &lcd, 
    int pin_echo, 
    int pin_trig, 
    NewPing &sonarUsed,
    int pirPinUsed,
    uint8_t analog_pin,
    float beta
)
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState, analog_pin, beta)
{
    this->pirPin = pirPinUsed;
    maxTemp = 50.0;
    initalTime = 0;
    T3 = 5000; // 5 seconds threshold for temperature alarm
}

IdleState::~IdleState()
{
}

void IdleState::enterState()
{
    clearScreen();
    turnOffAllLeds();
    changeLed(0);
    
    writeOnDisplay(0, 0, "DRONE INSIDE");
    closeMotor();
    
    // We are at 'R'est
    Serial.println('R');
}

bool IdleState::canEmergencyStop() const
{
    // Emergency stop is allowed in Idle state
    return true;
}

GenericState* IdleState::update()
{
    // Check for Overheating
    if (preAlarmStateCheck() || getAlarmState())
    {
        // Transition to PreAlarm
        return new PreAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirPin, analog_pin, beta, this->clone());
    }
    
    // Check for Take Off
    else if (readChar() == 'T')
    {
        return new TakeOffState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirPin, analog_pin, beta);
    }

    return nullptr;
}

void IdleState::exitState()
{
}

GenericState *IdleState::clone()
{
    return new IdleState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta);
}