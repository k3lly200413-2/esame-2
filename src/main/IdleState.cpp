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
    T3 = 5000;
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
    Serial.println('R');
}

bool IdleState::canEmergencyStop() const
{
    return true;
}

GenericState* IdleState::update()
{
    if (preAlarmStateCheck() || getAlarmState())
    {
        return new PreAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirPin, analog_pin, beta, this->clone());
        /* Don't Start new Flight */
    }
    else if (readChar() == 'T')
    {
        return new TakeOffState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirPin, analog_pin, beta);
    }

    return nullptr;

    /* you can start new flight */
}

void IdleState::exitState()
{
}

GenericState *IdleState::clone()
{
    return new IdleState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta);
}
