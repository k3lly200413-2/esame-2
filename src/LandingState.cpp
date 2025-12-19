#include "LandingState.h"

LandingState::LandingState(
    int leds[3], 
    Servo &servo, 
    LiquidCrystal_I2C &lcd, 
    int pin_echo, 
    int pin_trig, 
    NewPing &sonarUsed,
    int pirOutPin)
: GenericState(leds, servo, lcd, pin_echo, pin_trig, sonarUsed, pirState)
{
    D2 = 10;
    T2 = 4000;
    initialTime = 0;
    currentTime = 0;
    pirOutPinUsed = pirOutPin;
}

LandingState::~LandingState()
{
}

void LandingState::enterState()
{
    openMotor();
    digitalWrite(pirOutPinUsed, LOW);
    Serial.println("Entered Landing State!");
    Serial.println(digitalRead(pirOutPinUsed));

}

void LandingState::update()
{
    int val = digitalRead(pirOutPinUsed);
    if (val == HIGH)
    {
        if (pirState == LOW)
        {
            Serial.println("We're in the loop");
            Serial.print("Distance => ");
            Serial.println(getDistance());
            Serial.print("D2 => ");
            Serial.println(D2);
            if (getDistance() >= D2)
            {
                initialTime = 0;
            }
            else
            {
                Serial.println("Distance is fine");
                if (initialTime == 0)
                {
                    initialTime = millis();
                    
                }
                else
                {
                    currentTime = millis() - initialTime;
                    if (currentTime > T2)
                    {
                        closeMotor();
                        writeOnDisplay(0, 0, "DRONE INSIDE");
                    }
                }
            }
        }
        // openMotor();
        // writeOnDisplay(0, 0, "LANDING");
        // pirState = LOW;
    }
    else
    {
        if (pirState == HIGH)
        {
            Serial.println("Motion Ended");
            pirState = LOW;
        }
        
    }
}

void LandingState::exitState()
{
}
