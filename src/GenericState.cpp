#include "GenericState.h"

GenericState::GenericState(
    int leds[3],
    Servo &servo,
    LiquidCrystal_I2C &lcdRef)
: servoUsed(servo), lcd(lcdRef)
{
    for (int i = 0; i < 3; i ++)
    {
        ledPins[i] = leds[i];
    }
}

void GenericState::writeOnDisplay(int cursorX, int cursorY, char *text)
{
    lcd.setCursor(cursorX, cursorY);
    lcd.print(text);
}

void GenericState::openMotor()
{
    servoUsed.write(180);
}

void GenericState::closeMotor()
{
    servoUsed.write(0);
}

void GenericState::changeLed(int ledIndex, int newLedState)
{
    digitalWrite(ledPins[ledIndex], newLedState);
}

void GenericState::turnOnAllLeds()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(ledPins[i], HIGH);
    }
}

void GenericState::turnOffAllLeds()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(ledPins[i], LOW);
    }
}
