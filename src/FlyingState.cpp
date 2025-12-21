#include "FlyingState.h"
#include "LandingState.h"

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
    
    // Initialize as false. We won't trust the sensor 
    // until we verify it has gone LOW at least once.
    sensorReset = false; 
    
    // Ensure the pin is set to INPUT (best practice to do this in setup(), but good to verify)
    pinMode(pirPin, INPUT); 
}

FlyingState::~FlyingState()
{
}

void FlyingState::enterState()
{
    turnOffAllLeds();
    clearScreen();
    writeOnDisplay(0, 0, "ENTERED FLYING STATE");
}

bool FlyingState::canEmergencyStop() const
{
    return false;
}

GenericState* FlyingState::update()
{
    if (getAlarmState())
    {
        writeOnDisplay(0, 0, "NUH HUH");
        return NULL;
    }
    int val = digitalRead(pirPin);
    
    Serial.println(pirPin);

    if (val == HIGH)
    {        
        // Update internal state tracking
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
    Serial.println("Exited Idle State");
}
