#include "GenericState.h"
#include<Arduino.h>
#include<LiquidCrystal_I2C.h>
#include<Servo.h>

/*
Available functions not declaired:
    changeLed
    turnOnAllLeds
    turnOffAllLeds
*/


GenericState::GenericState(
    int leds[3],
    Servo &servo,
    LiquidCrystal_I2C &lcdDisplay
)

    // Understand why this is here, apparently because of the & so that we don't create a copy when passing the reference
    : lcd(lcdDisplay), servoUsed(servo)
{
    ledPins[0] = leds[0];
    ledPins[1] = leds[1];
    ledPins[2] = leds[2];

    for(int i=0; i<3; i++) {
        pinMode(ledPins[i], OUTPUT);
    }

}

void GenericState::enterState()
{
    turnOffAllLeds();
    changeLed(0, HIGH);
    writeOnDisplay(0, 0, "DRONE INSIDE");
    closeMotor();
    Serial.println("Entered Drone Inside State");
}

void GenericState::update()
{}

void GenericState::button()
{}

void GenericState::display()
{}

void GenericState::pir()
{}

void GenericState::sonar()
{}

void GenericState::temperature()
{}

void GenericState::exitState()
{}
