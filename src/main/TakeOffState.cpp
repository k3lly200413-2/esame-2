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
    T1 = 10000;      // Time threshold: Drone must stay above D1 for 10 seconds
    D1 = 30;         // Distance threshold: 30cm
    
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
    
    // Open the hangar door to allow exit
    openMotor();
    
    writeOnDisplay(0, 0, "TAKE OFF");
    
    // Notify Python script: 'T' = Take Off Phase
    Serial.println('T');
}

bool TakeOffState::canEmergencyStop() const
{
    // Do not allow stops during critical take-off phase
    return false;
}

GenericState* TakeOffState::update()
{
    // Background safety check for temperature
    preAlarmStateCheck();

    // --- NON-BLOCKING LED BLINK ---
    // Toggle Yellow LED (Index 1) every 500ms
    if (millis() - lastBlinkTime >= 500) 
    {
        lastBlinkTime = millis();
        changeLed(1);
    }

    // --- DISTANCE LOGIC ---
    unsigned long distance = getDistance();

    // Filter out sensor noise (0 usually means "out of range", >400 is unlikely)
    if (distance <= 0 || distance > 400) 
    {
        return NULL; 
    }

    // Logic: The drone must be FAR away (Distance > D1) for a set time (T1) 
    // to confirm it has successfully left the hangar.

    if (distance < D1)
    {
        // Drone is still close/inside. Reset the timer.
        initialTime = 0;
    }
    else 
    {
        // Drone is far away (above threshold)
        if (initialTime == 0)
        {
            // Start the timer
            initialTime = millis();
        }
        else
        {
            // Check how long it has been away
            unsigned long elapsedTime = millis() - initialTime;
            
            if (elapsedTime >= T1)
            {
                // Confirmed: Drone has left.
                closeMotor();
                writeOnDisplay(0, 0, "DRONE OUT");
                
                // Transition to Flying State
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