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
    D2 = 10;        // Distance threshold (cm) to confirm drone has landed
    T2 = 4000;      // Time threshold (ms) drone must stay below D2 to be "Landed"
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
    
    // Open the hanger for the drone
    openMotor();
    
    // Reset PIR output just in case
    digitalWrite(pirOutPinUsed, LOW);
    
    writeOnDisplay(0, 0, "LANDING");
    
    lastBlinkTime = millis();
    
    // Notify Python script: 'L' = Landing Phase
    Serial.println('L');
}

bool LandingState::canEmergencyStop() const
{
    // Do not allow stops during critical landing phase
    return false;
}

GenericState* LandingState::update()
{
    // Send Distance data to Python for plotting ("DIST 12")
    Serial.print("DIST ");
    Serial.println(getDistance());
    
    // Check for temperature alarm (background check)
    preAlarmStateCheck();

    // --- NON-BLOCKING LED BLINK ---
    // Toggle the yellow LED (Index 1) every 500ms
    if (millis() - lastBlinkTime >= 500) 
    {
        lastBlinkTime = millis();
        isLedOn = !isLedOn;
        changeLed(1);
    }

    // --- LANDING CONFIRMATION LOGIC ---
    // If distance is ABOVE threshold D2, the drone is not settled yet.
    if (getDistance() >= D2)
    {
        // Reset the timer
        initialTime = 0;
    }
    else
    {
        // Drone is BELOW threshold (close to floor)
        if (initialTime == 0)
        {
            // Start the timer
            initialTime = millis();
        }
        else
        {
            // Calculate how long it has been settled
            currentTime = millis() - initialTime;
            
            // If settled for longer than T2 (4 seconds), confirm landing
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