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
    GenericState* previousState
)
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState, analog_pin, beta)
{
    maxTemp2 = 70;
    T4 = 3000;
    this->previousState = previousState;
}

PreAlarmState::~PreAlarmState()
{
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
    Serial.print("Temp => ");
    Serial.println(getTemp());
    if (getTemp() < maxTemp2)
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
            if (elapsedTime >= T4)
            {
                return new FullAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta, previousState);
            }
        }
    }
    return NULL;
}

void PreAlarmState::exitState()
{
    Serial.println("Exiting PreAlarm State!");
}