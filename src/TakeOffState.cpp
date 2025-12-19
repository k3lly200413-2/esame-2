#include "TakeOffState.h"

TakeOffState::TakeOffState(
    int leds[3],
    Servo &servo,
    LiquidCrystal_I2C &lcd,
    int pin_echo,
    int pin_trig,
    NewPing &sonarUsed,
    int pirState
)
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState)
{
    initialTime = 0;
    T1 = 10000;
    D1 = 30;
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
    openMotor();
    writeOnDisplay(0, 0, "TAKE OFF");
}

void TakeOffState::update()
{
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
            
            Serial.print("Elapsed Time => ");
            Serial.println(elapsedTime);

            // Debugging (Note: String concat with numbers doesn't work well in C++)
            if (elapsedTime >= T1)
            {
                closeMotor();
                writeOnDisplay(0, 0, "DRONE OUT");
                
                // Optional: You might want to switch states here 
                // or add a flag so this doesn't write to the LCD repeatedly
            }
        }
    }
}

void TakeOffState::exitState()
{}
