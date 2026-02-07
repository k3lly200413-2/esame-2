#include "GenericState.h"
#include "PreAlarmState.h"
#include <NewPing.h>
#include "DHT11.h"

// Initialize static member
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
: servoUsed(servo), lcd(lcdRef), echo_pin(pin_echo), trig_pin(pin_trig), sonar(sonarUsed), analog_pin(analog_pin), beta(beta), sensorReader(13)
{
    maxTemp = 50;
    T3 = 3000; // 3 seconds delay before alarm triggers
    
    for (int i = 0; i < 3; i++)
    {
        ledPins[i] = leds[i];
    }
}

bool GenericState::canEmergencyStop() const
{
    // Default behavior: States allow emergency stops unless overridden
    return true;
}

/**
 * Writes text to the LCD with automatic line wrapping.
 * If text exceeds the first line (16 chars), it jumps to the second line.
 * @param cursorX Starting X position
 * @param cursorY Starting Y position
 * @param text The string to display
 */
void GenericState::writeOnDisplay(int cursorX, int cursorY, char *text)
{
    lcd.setCursor(cursorX, cursorY);
    
    int currentX = cursorX;
    int currentY = cursorY;

    for (int i = 0; text[i] != '\0'; i++) 
    {
        // Check if we reached the end of the row (16 columns)
        if (currentX >= 16) 
        {
            currentX = 0;      // Reset X to start of line
            currentY = 1;      // Force jump to the second line
            lcd.setCursor(currentX, currentY);
        }

        lcd.print(text[i]);
        currentX++;
    }
}

/**
 * Moves servo to 0 degrees (Open position)
 */
void GenericState::openMotor()
{
    servoUsed.write(0);
}

/**
 * Moves servo to 90 degrees (Closed position)
 */
void GenericState::closeMotor()
{
    servoUsed.write(90);
}

/**
 * Toggles the state of a specific LED (Index 0-2)
 */
void GenericState::changeLed(int ledIndex)
{
    int newLedState;
    
    // Read current state and invert it
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

/**
 * Gets distance from ultrasonic sensor.
 * Includes delay to prevent signal interference.
 */
unsigned long GenericState::getDistance()
{
    delay(50); 
    unsigned long distance = sonar.ping_cm();
    return distance;
}

/**
 * Reads temperature from the sensor (DHT11 or similar).
 */
int GenericState::getTemp()
{
    return sensorReader.readTemperature();
}

/**
 * Checks if the system needs to enter PreAlarm state based on temperature.
 * Implements a time-delay (hysteresis) to avoid false alarms.
 * * @return true if alarm condition is met (Temperature > maxTemp for > T3 seconds)
 */
bool GenericState::preAlarmStateCheck()
{
    if (getTemp() < maxTemp)
    {
        // Temperature is normal, reset the timer
        initalTime = 0;
    }
    else
    {
        // Temperature is high
        if (initalTime == 0)
        {
            // First time detecting high temp, start the timer
            initalTime = millis();
        }
        else
        {
            // Check how long it has been high
            unsigned long elapsedTime = millis() - initalTime;
            
            if (elapsedTime >= T3)
            {
                // Threshold exceeded, trigger alarm
                setAlarmState(true);
                return true; 
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

/**
 * Reads a single character from the Serial buffer (from Python script)
 */
const char GenericState::readChar()
{
    return Serial.read();
}