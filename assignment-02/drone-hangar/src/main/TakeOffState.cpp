#include "TakeOffState.h"
#include "FlyingState.h"

TakeOffState::TakeOffState(
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
    initialTime = 0;
    T1 = 10000;
    D1 = 30;
    
    lastBlinkTime = 0;
    isLedOn = false;
    pirPin = pirPinUsed;
}

TakeOffState::~TakeOffState()
{}

void TakeOffState::enterState()
{
    turnOffAllLeds();   
    clearScreen();
    
    openMotor();
    
    writeOnDisplay(0, 0, "TAKE OFF");
    
    // Notify Python script: 'T'ake Off 
    Serial.println('T');
}

bool TakeOffState::canEmergencyStop() const
{
    return false;
}

GenericState* TakeOffState::update()
{
    preAlarmStateCheck();

    if (millis() - lastBlinkTime >= 500) 
    {
        lastBlinkTime = millis();
        changeLed(1);
    }

    unsigned long distance = getDistance();

    // Filter out sensor noise
    if (distance <= 0 || distance > 400) 
    {
        return NULL; 
    }

    if (distance < D1)
    {
        initialTime = 0;
    }
    else 
    {
        // Drone is far away
        if (initialTime == 0)
        {
            initialTime = millis();
        }
        else
        {
            // Check how long it has been away
            unsigned long elapsedTime = millis() - initialTime;
            
            if (elapsedTime >= T1)
            {
                // Drone has left.
                closeMotor();
                writeOnDisplay(0, 0, "DRONE OUT");
                
                // Flying State
                return new FlyingState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirPin, analog_pin, beta);
            }
        }
    }
    return NULL;
}

void TakeOffState::exitState()
{}

GenericState *TakeOffState::clone()
{
    return new TakeOffState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta);
}