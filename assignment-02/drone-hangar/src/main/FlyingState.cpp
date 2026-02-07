#include "HardwareSerial.h"
#include "FlyingState.h"
#include "LandingState.h"
#include "PreAlarmState.h"

FlyingState::FlyingState(
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
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirPinUsed, analog_pin, beta)
{
    pirPin = pirPinUsed;
    sensorReset = false; 
}

FlyingState::~FlyingState()
{
}

void FlyingState::enterState()
{
    turnOffAllLeds();
    clearScreen();
    writeOnDisplay(0, 0, "DRONE OUT");
    
    // We are 'O'perating
    Serial.println('O');
}

bool FlyingState::canEmergencyStop() const
{
    // Emergency stop is disabled while flying to prevent crashes
    return false;
}

GenericState* FlyingState::update()
{
    // if temperature is too high stop
    if (getAlarmState())
    {   
        // Placeholder text for future warning if needed
        writeOnDisplay(0, 0, "Watch out!");
        
        // Transition to PreAlarm, passing clone to return to this state
        return new PreAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirPin, analog_pin, beta, this->clone());
    }

    // 2. Check for Landing Conditions
    int val = digitalRead(pirPin);

    // If presence is detected and python sends Land command then land
    if (val == HIGH && readChar() == 'L')
    {        
        // Ensure state change only happens on the rising edge of the logic
        if (pirState == LOW) 
        {
            pirState = HIGH;
            return new LandingState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirPin, analog_pin, beta);
        }
    }
    else
    {
        pirState = LOW;    
    }

    return NULL;
}

void FlyingState::exitState()
{
}

GenericState* FlyingState::clone()
{
    // Create a clone of this state to allow to return to this state
    return new FlyingState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirPin, analog_pin, beta);
}