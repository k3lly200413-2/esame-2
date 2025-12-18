#include "TakeOffState.h"

TakeOffState::TakeOffState(
    int leds[3],
    Servo &servo,
    LiquidCrystal_I2C &lcd
)
: GenericState(leds, servo, lcd)
{
    initialTime = 0;
    T1 = 60000;
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
        if(initialTime == 0)
        {
            initialTime = millis();
        }
        else
        {
            if (millis() - initialTime >= T1)
            {
                closeMotor();
                writeOnDisplay(0, 0, "DRONE OUT");
            }
        }
    }
}

void TakeOffState::exitState()
{}
