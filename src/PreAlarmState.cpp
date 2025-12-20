#include "GenericState.h"
#include "PreAlarmState.h"

PreAlarmState::PreAlarmState(
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
{}

PreAlarmState::~PreAlarmState()
{
}

void PreAlarmState::enterState()
{
    Serial.println("Entered PreAlarm State!");
}

GenericState* PreAlarmState::update()
{
    return NULL;
}

void PreAlarmState::exitState()
{
    Serial.println("Exiting PreAlarm State!");
}