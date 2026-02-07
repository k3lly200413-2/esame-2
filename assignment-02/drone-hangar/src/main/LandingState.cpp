#include "LandingState.h"
#include "IdleState.h"

LandingState::LandingState(
    int leds[3], 
    Servo &servo, 
    LiquidCrystal_I2C &lcd, 
    int pin_echo, 
    int pin_trig, 
    NewPing &sonarUsed,
    int pirOutPin,
    uint8_t analog_pin,
    float beta
)
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState, analog_pin, beta)
{
    D2 = 10;        // Distance to confirm drone has landed
    T2 = 4000;      // Time drone must stay below D2 to be "Landed"
    initialTime = 0;
    currentTime = 0;
    pirOutPinUsed = pirOutPin;
    
    lastBlinkTime = 0;
    isLedOn = false;
}

LandingState::~LandingState()
{
}

void LandingState::enterState()
{
    clearScreen();
    openMotor();
    
    // Reset PIR output just in case
    digitalWrite(pirOutPinUsed, LOW);
    
    writeOnDisplay(0, 0, "LANDING");
    
    lastBlinkTime = millis();
    
    // Notify Python script: 'L'anding
    Serial.println('L');
}

bool LandingState::canEmergencyStop() const
{
    // Cannot stop during emergency
    return false;
}

GenericState* LandingState::update()
{
    // Send Distance data to Python for plotting
    Serial.print("DIST ");
    Serial.println(getDistance());
    
    // Check for temperature alarm
    // if it's true it will be turned into a alarm state in idle state
    // this will potentially set the alarm flag to true
    preAlarmStateCheck();

    // Blinking
    if (millis() - lastBlinkTime >= 500) 
    {
        lastBlinkTime = millis();
        isLedOn = !isLedOn;
        changeLed(1);
    }

    // if distance is >= D2 then don't start the timer to see if the drone is inside
    if (getDistance() >= D2)
    {
        // Reset the timer
        initialTime = 0;
    }
    else
    {
        // Drone is close to the floor
        if (initialTime == 0)
        {
            // Start the timer
            initialTime = millis();
        }
        else
        {
            // Calculate how long it has been 
            currentTime = millis() - initialTime;
            
            // If close to the floor for longer than T2 seconds
            if (currentTime > T2)
            {
                closeMotor();
                writeOnDisplay(0, 0, "DRONE INSIDE");
                
                // Transition to Idle State
                return new IdleState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta);
            }
        }
    }
    return NULL;
}

void LandingState::exitState()
{
    turnOffAllLeds();
}

GenericState *LandingState::clone()
{
    return new LandingState(ledPins, servoUsed, lcd, echo_pin, trig_pin, sonar, pirState, analog_pin, beta);
}