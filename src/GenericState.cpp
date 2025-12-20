#include "GenericState.h"
#include<NewPing.h>

GenericState::GenericState(
    int leds[3],
    Servo &servo,
    LiquidCrystal_I2C &lcdRef,
    int pin_echo,
    int pin_trig,
    NewPing &sonarUsed,
    int pirState,
    uint8_t analog_pin,
    float beta
)
: servoUsed(servo), lcd(lcdRef), echo_pin(pin_echo), trig_pin(pin_trig), sonar(sonarUsed), analog_pin(analog_pin), beta(beta)
{
    for (int i = 0; i < 3; i++)
        ledPins[i] = leds[i];
}

void GenericState::writeOnDisplay(int cursorX, int cursorY, char *text)
{
    lcd.setCursor(cursorX, cursorY);
    lcd.print(text);
}

void GenericState::openMotor()
{
    servoUsed.write(0);
}

void GenericState::closeMotor()
{
    servoUsed.write(90);
}

void GenericState::changeLed(int ledIndex)
{
    int newLedState;
    if (digitalRead(ledPins[ledIndex]) == HIGH)
    {
        newLedState = LOW;
    }
    else
    {
        newLedState = HIGH;
    }
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

unsigned long GenericState::getDistance()
{
    delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    unsigned long distance = sonar.ping_cm();
    return distance;
}

float GenericState::getTemp()
{
    Serial.print("AnalogRead => ");
    Serial.println(analogRead(analog_pin));
    Serial.print("beta => ");
    Serial.println(beta);
    return 1 / ( log( 1 /( 1023. / analogRead(analog_pin) - 1 )) / beta + 1.0 / 298.15 ) - 273.15;
}