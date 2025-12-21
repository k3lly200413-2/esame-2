#include <Servo.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "GenericState.h"
#include "DroneInsideState.h"
#include "TakeOffState.h"
#include "LandingState.h"
#include "IdleState.h"

// --- PIN DEFINITIONS ---
#define I2C_ADDRESS 0x27
#define PIN_TRIG 8
#define PIN_ECHO 7
#define SERVO_PIN 3  // Note: On ESP32, Pin 3 is RX0 (High at boot). Use Pin 4, 5, or 13 if possible.
#define TEMP_PIN A0

// CHANGED: Moved PIR to Pin 12 to avoid conflict with Pin 2 (Onboard LED/Strapping Pin)
const int pirOutPin = 12; 

// CHANGED: Button is on Pin 9 (as per your variable), not 6!
const int buttonPin = 9;  

// LEDs on 4, 5, 6
int ledPins[] = { 4, 5, 6 };

const float BETA = 3950;

Servo myServo;
LiquidCrystal_I2C lcd(I2C_ADDRESS, 16, 2);
NewPing sonar(PIN_TRIG, PIN_ECHO);
GenericState* initialState;
StateManager* stateManager;

void setup()
{
    Serial.begin(9600);
    
    // 1. Initialize Display
    lcd.init();
    lcd.backlight();

    // 2. Initialize Servo
    myServo.attach(SERVO_PIN);
    // REMOVED: pinMode(3, OUTPUT); -> Servo library handles this.

    // 3. Initialize Sensors
    pinMode(pirOutPin, INPUT); 
    pinMode(buttonPin, INPUT); // Changed from 6 to buttonPin (9)

    // 4. Initialize LEDs
    // We loop through the array to prevent mistakes
    for (int i = 0; i < 3; i++) {
        pinMode(ledPins[i], OUTPUT);
    }

    // 5. Initialize State Machine
    Serial.print("PIR PIN => ");
    Serial.print(pirOutPin);
    initialState = new IdleState(ledPins, myServo, lcd, PIN_ECHO, PIN_TRIG, sonar, pirOutPin, TEMP_PIN, BETA);
    stateManager = new StateManager(initialState);
    stateManager->init();
    
    Serial.println("System Initialized. Waiting for PIR...");
}

void loop()
{
    stateManager->update();
    // Small delay helps Wokwi simulation stability
    // delay(10); 
}