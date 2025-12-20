#include "IdleState.h"
#include "PreAlarmState.hpp"

IdleState::IdleState(
    int leds[3],
    Servo &servo, 
    LiquidCrystal_I2C &lcd, 
    int pin_echo, 
    int pin_trig, 
    NewPing &sonarUsed,
    int pirState,
    uint8_t analog_pin,
    int beta
)
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState, analog_pin, beta)
{
    maxTemp = 100.0;
    initalTime = 0;
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
    // Serial.println("Updating Idle State. Doing Nothing");
    if (getTemp() < maxTemp)
    {
        initalTime = 0;
    }
    else
    {
        if (initalTime == 0)
        {
            initalTime = millis();
        }
        else
        {
            int elapsedTime = millis() - initalTime;
            if (elapsedTime >= T3)
            {
                return new PreAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta);
            }
        }
    }
    return NULL;
}

void IdleState::exitState()
{
    Serial.println("Exited Idle State");
}
