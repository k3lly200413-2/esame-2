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
    /*
    Once we enter the Take Off State we need to:
        Open the Servo
        Write "TAKE OFF" on the LCD
        Check if the distance of the drone is >= D1 for more than T1 Time
        if it is:
            Close Servo
            Write "DRONE OUT" on the LCD
        otherwise:
            Do nothing I guess?
    */
    clearScreen();
    openMotor();
    writeOnDisplay(0, 0, "TAKE OFF");
}

bool TakeOffState::canEmergencyStop() const
{
    return false;
}

GenericState* TakeOffState::update()
{
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

    unsigned long distance = getDistance();
    // For Noise maybe
    if (distance <= 0 || distance > 400) 
    {
        return; 
    }

    // 1. Check if the condition is BROKEN (Drone is too low)
    if (distance < D1)
    {
        // Reset the timer flag because distance requirement isn't met
        initialTime = 0;
    }
    // 2. Condition IS met (Drone is high enough)
    else 
    {
        // If the timer hasn't started yet, start it now
        if (initialTime == 0)
        {
            initialTime = millis();
        }
        // If the timer is already running, check how much time has passed
        else
        {
            unsigned long elapsedTime = millis() - initialTime;
            
            // Serial.print("Elapsed Time => ");
            // Serial.println(elapsedTime);

            // Debugging (Note: String concat with numbers doesn't work well in C++)
            if (elapsedTime >= T1)
            {
                closeMotor();
                writeOnDisplay(0, 0, "DRONE OUT");
                Serial.println(pirState);
                return new FlyingState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirPin, analog_pin, beta);
            }
        }
    }
    return NULL;
}

void TakeOffState::exitState()
{}
