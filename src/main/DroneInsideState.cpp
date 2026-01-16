#include "DroneInsideState.h"
#include "IdleState.h"

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
    int pin_trig,
    NewPing &sonarUsed,
    int pirState,
    uint8_t analog_pin,
    float beta
)

    // : is a list of constructors, used to construct the variables before the constructor of the class in question
    // lcd is a pointer, because of this we need to pass an initialised version of the class ( I think ) so we need to understand when we need to initialise the first pointer to the liquidcristali2c class
    : GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState, analog_pin, beta)
{
}

DroneInsideState::~DroneInsideState()
{
}

void DroneInsideState::enterState()
{
    // Clear screen from previous message
    clearScreen();
    // Turn off all leds from before
    turnOffAllLeds();
    // Turn on only the first 
    changeLed(0);
    // Write on the display
    writeOnDisplay(0, 0, "DRONE INSIDE");
    // Close the motor
    closeMotor();
    // Serial.flush();
    Serial.println('R');
}

GenericState* DroneInsideState::update()
{
    return new IdleState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta);
}

void DroneInsideState::exitState()
{
    lcd.clear();
}

GenericState *DroneInsideState::clone()
{
    return new DroneInsideState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta);
}
