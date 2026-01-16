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
    
    // Notify Python script that we are currently 'O'perating
    Serial.println('O');
}

bool FlyingState::canEmergencyStop() const
{
    // Emergency stop is disabled while flying to prevent crashes
    return false;
}

GenericState* FlyingState::update()
{
    // 1. Check for Critical Temperature Alarm
    if (getAlarmState())
    {   
        writeOnDisplay(0, 0, "NUH HUH"); // Placeholder text for alarm
        
        // Transition to PreAlarm, passing a clone of 'this' to remember previous state
        return new PreAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirPin, analog_pin, beta, this->clone());
    }

    // 2. Check for Landing Conditions
    int val = digitalRead(pirPin);

    // Only land if PIR detects a person AND the user sends the 'L' command
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
    // Create a copy of the current state (Prototype Pattern)
    return new FlyingState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirPin, analog_pin, beta);
}