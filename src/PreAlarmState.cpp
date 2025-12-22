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
    maxTemp2 = 70;
    maxTemp1 = 60;
    T4 = 3000;
    this->returnTicket = stateToReturnTo;
}

PreAlarmState::~PreAlarmState()
{
    if (returnTicket != NULL) {
        delete returnTicket;
    }
}

void PreAlarmState::enterState()
{
    clearScreen();
    writeOnDisplay(0, 0, "WE IN CRISIS");
    Serial.println("Entered PreAlarm State!");
}

bool PreAlarmState::canEmergencyStop() const
{
    return false;
}

GenericState* PreAlarmState::update()
{
    float currentTemp = getTemp();
    Serial.print("Temp => ");
    Serial.println(currentTemp);

    // 1. IS IT SAFE?
    if (currentTemp < maxTemp1)
    {
        initalTime = 0; // Reset timer just in case
        
        // --- THIS WAS MISSING ---
        GenericState* temp = returnTicket;
        setAlarmState(false);
        returnTicket = NULL; // Prevent destructor from killing it
        Serial.println("Temp is safe. Returning to previous state.");
        return temp; 
        // ------------------------
    }
    // 2. IS IT DANGEROUS?
    else if (currentTemp > maxTemp2)
    {
        if (initalTime == 0)
        {
            initalTime = millis();
        }
        else if (millis() - initalTime >= T4)
        {
            // Time is up! Go to Full Alarm
            GenericState* temp = returnTicket; 
            returnTicket = NULL;
            return new FullAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta, temp);
        }
    }

    return NULL; // Stay in PreAlarm
}

void PreAlarmState::exitState()
{
    Serial.println("Exiting PreAlarm State!");
}

GenericState *PreAlarmState::clone()
{
    return new PreAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta, this->clone());
}
