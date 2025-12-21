#include "GenericState.h"
#include "PreAlarmState.h"
#include<NewPing.h>

bool GenericState::alarmState = false;

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
    // bool alarmState;
    maxTemp = 50;
    T3 = 3000;
    for (int i = 0; i < 3; i++)
    {
        ledPins[i] = leds[i];
    }

}

bool GenericState::canEmergencyStop() const
{
    return true;
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
    // Serial.print("AnalogRead => ");
    // Serial.println(analogRead(analog_pin));
    // Serial.print("beta => ");
    // Serial.println(beta);
    float tempSensorValue = analogRead(analog_pin);
    return 1 / ( log( 1 /( 1023. / tempSensorValue - 1 )) / beta + 1.0 / 298.15 ) - 273.15;
}

bool GenericState::preAlarmStateCheck()
{

    if (getTemp() < maxTemp)
    {
        initalTime = 0;
    }
    else
    {
        if (initalTime == 0)
        {
            initalTime = millis();
        }
        else
        {
            unsigned long elapsedTime = millis() - initalTime;
            Serial.println(elapsedTime);
            if (elapsedTime >= T3)
            {
                Serial.print("PreAlarmState => true");
                setAlarmState(true);
                return true; /* new PreAlarmState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta);*/
            }
        }
    }
    return false;
}

void GenericState::setAlarmState(bool newState)
{
    alarmState = newState;
}

bool GenericState::getAlarmState()
{
    return alarmState;
}

void GenericState::clearScreen()
{
    lcd.clear();
}
