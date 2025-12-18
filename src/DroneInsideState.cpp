#include "DroneInsideState.h"

/*
Available functions not declaired:
    changeLed
    turnOnAllLeds
    turnOffAllLeds
*/


DroneInsideState::DroneInsideState(
    int leds[3],
    Servo &servo,
    LiquidCrystal_I2C &lcd,
    int pin_echo,
    int pin_trig
)

    // : is a list of constructors, used to construct the variables before the constructor of the class in question
    // lcd is a pointer, because of this we need to pass an initialised version of the class ( I think ) so we need to understand when we need to initialise the first pointer to the liquidcristali2c class
    : GenericState(leds, servo, lcd, pin_echo, pin_trig)
{
}

DroneInsideState::~DroneInsideState()
{
}

void DroneInsideState::enterState()
{
    turnOffAllLeds();
    changeLed(0, HIGH);
    writeOnDisplay(0, 0, "DRONE INSIDE");
    closeMotor();
    Serial.println("Entered Drone Inside State");
}

void DroneInsideState::update()
{}

void DroneInsideState::exitState()
{
    lcd.clear();
}
