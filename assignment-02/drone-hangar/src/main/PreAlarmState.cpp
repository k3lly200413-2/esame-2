#include "GenericState.h"
#include "PreAlarmState.h"
#include "FullAlarmState.h"

PreAlarmState::PreAlarmState(
    int leds[3],
    Servo &servo,
    LiquidCrystal_I2C &lcd,
    int pin_echo,
    int pin_trig,
    NewPing &sonarUsed,
    int pirState,
    uint8_t analog_pin,
    float beta,
    GenericState* stateToReturnTo
)
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState, analog_pin, beta)
{
    // Temperature Thresholds
    maxTemp2 = 70; // Critical level
    maxTemp1 = 60; // Uh Oh level
    T4 = 3000;     // Duration temp must remain critical before triggering alarm
    
    // Save previous state so we can return to it 
    this->returnTicket = stateToReturnTo;
}

PreAlarmState::~PreAlarmState()
{
    // memory leak prevention
    if (returnTicket != NULL) {
        delete returnTicket;
    }
}

void PreAlarmState::enterState()
{
}

bool PreAlarmState::canEmergencyStop() const
{
    // Cannot stop during a safety check transition
    return false;
}

GenericState* PreAlarmState::update()
{
    float currentTemp = getTemp();

    // if level is less than Uh Oh level then cool
    if (currentTemp < maxTemp1)
    {
        initalTime = 0;
        
        // get the previous state
        GenericState* temp = returnTicket;
        
        // Reset flag to allow normal operations
        setAlarmState(false);
        
        // Detach the pointer from 'this' object so the destructor doesn't delete it
        returnTicket = NULL; 
        
        // Return to the previous state (e.g., resume Flying)
        return temp; 
    }
    
    // if temperature is more than critical 
    else if (currentTemp > maxTemp2)
    {
        if (initalTime == 0)
        {
            initalTime = millis();
        }
        else if (millis() - initalTime >= T4)
        {
            // Full Alarm

            GenericState* temp = returnTicket; 
            returnTicket = NULL; // Detach pointer
            
            return new FullAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta, temp);
        }
    }

    // If nothing stay in state
    return NULL; 
}

void PreAlarmState::exitState()
{
}

GenericState *PreAlarmState::clone()
{
    // When cloning, we must also clone the state we are holding a ticket for
    return new PreAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta, this->clone());
}