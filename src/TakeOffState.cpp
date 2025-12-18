#include "TakeOffState.h"

TakeOffState::TakeOffState(
    int leds[3],
    Servo &servo,
    LiquidCrystal_I2C &lcd
)
: GenericState(leds, servo, lcd)
{
    initialTime = 0;
    T1 = 10000;
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
    if (1 /*Some distance*/)
    {
        // Serial.println("Update");
        if(initialTime == 0)
        {
            initialTime = millis();
            currentTime = initialTime;
        }
        else
        {
          Serial.println(currentTime);
            if (currentTime - initialTime >= T1)
            {
                Serial.println("Time Passed => " + millis() - initialTime);
                closeMotor();
                writeOnDisplay(0, 0, "DRONE OUT");
            }
            currentTime = millis();
        }
    }
}

void TakeOffState::exitState()
{}
