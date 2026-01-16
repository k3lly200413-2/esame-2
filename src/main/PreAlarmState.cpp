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
    maxTemp2 = 70; // Critical threshold (triggers Full Alarm)
    maxTemp1 = 60; // Safe threshold (returns to Previous State)
    T4 = 3000;     // Duration temp must remain critical before triggering alarm
    
    // Save the state we came from (e.g., Idle or Flying) so we can return to it later
    this->returnTicket = stateToReturnTo;
}

PreAlarmState::~PreAlarmState()
{
    // Safety check: If we are destroyed without returning the ticket, delete it to prevent leaks.
    if (returnTicket != NULL) {
        delete returnTicket;
    }
}

void PreAlarmState::enterState()
{
    // Pre-Alarm usually runs silently in the background or maintains previous visuals
}

bool PreAlarmState::canEmergencyStop() const
{
    // Cannot stop during a safety check transition
    return false;
}

GenericState* PreAlarmState::update()
{
    float currentTemp = getTemp();

    // Condition 1: Temperature has dropped back to Safe Levels (< 60)
    if (currentTemp < maxTemp1)
    {
        initalTime = 0; // Reset timer
        
        // Retrieve the stored previous state
        GenericState* temp = returnTicket;
        
        // Reset flag to allow normal operations
        setAlarmState(false);
        
        // Detach the pointer from 'this' object so the destructor doesn't delete it
        returnTicket = NULL; 
        
        // Return to the previous state (e.g., resume Flying)
        return temp; 
    }
    
    // Condition 2: Temperature is Critical (> 70)
    else if (currentTemp > maxTemp2)
    {
        if (initalTime == 0)
        {
            // Start hysteresis timer
            initalTime = millis();
        }
        else if (millis() - initalTime >= T4)
        {
            // Timer expired: Escalate to Full Alarm
            
            // Pass the original state (returnTicket) forward to the Full Alarm
            // so we can eventually go back to it after the alarm is cleared.
            GenericState* temp = returnTicket; 
            returnTicket = NULL; // Detach pointer
            
            return new FullAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta, temp);
        }
    }

    // If neither condition is met (Temperature between 60-70), stay in PreAlarm (Return NULL)
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