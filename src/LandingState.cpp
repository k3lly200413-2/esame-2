#include "LandingState.h"
#include "IdleState.h"

LandingState::LandingState(
    int leds[3], 
    Servo &servo, 
    LiquidCrystal_I2C &lcd, 
    int pin_echo, 
    int pin_trig, 
    NewPing &sonarUsed,
    int pirOutPin,
    uint8_t analog_pin,
    float beta
)
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState, analog_pin, beta)
{
    D2 = 10;
    T2 = 4000;
    initialTime = 0;
    currentTime = 0;
    pirOutPinUsed = pirOutPin;
    lastBlinkTime = 0;
    isLedOn = false;
}

LandingState::~LandingState()
{
}

void LandingState::enterState()
{
    clearScreen();
    openMotor();
    digitalWrite(pirOutPinUsed, LOW);
    writeOnDisplay(0, 0, "WE LANDING");
    lastBlinkTime = millis();
}

bool LandingState::canEmergencyStop() const
{
    return false;
}

GenericState* LandingState::update()
{
    Serial.println("We in da loop");
    preAlarmStateCheck();
    if (millis() - lastBlinkTime >= 500) // 500ms = 0.5 seconds
    {
        // 1. Update the timer
        lastBlinkTime = millis();
        
        // 2. Toggle the state (If ON make OFF, If OFF make ON)
        isLedOn = !isLedOn;
        
        // 3. Write to the LED (Using the first LED in your list)
        changeLed(1);
    }

    if (getDistance() >= D2)
    {
        initialTime = 0;
    }
    else
    {
        Serial.println("Distance is fine");
        if (initialTime == 0)
        {
            initialTime = millis();
        }
        else
        {
            currentTime = millis() - initialTime;
            if (currentTime > T2)
            {
                closeMotor();
                writeOnDisplay(0, 0, "DRONE INSIDE");
                return new IdleState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta);
            }
        }
    }
    return NULL;
}

void LandingState::exitState()
{
    turnOffAllLeds();
}

GenericState *LandingState::clone()
{
    return new LandingState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta);
}
