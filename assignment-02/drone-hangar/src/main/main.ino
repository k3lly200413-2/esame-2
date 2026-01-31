#include <Servo.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "GenericState.h"
#include "DroneInsideState.h"
#include "TakeOffState.h"
#include "LandingState.h"
#include "IdleState.h"
#include "StateManagerHeader.h" 

// --- PIN DEFINITIONS ---
#define I2C_ADDRESS 0x27
#define PIN_TRIG 8
#define PIN_ECHO 7
#define SERVO_PIN 3 
#define TEMP_PIN 13

const int pirOutPin = 12; 
const int buttonPin = 9;  

// LEDs on Pins 4, 5, 6
int ledPins[] = { 4, 5, 6 };

const float BETA = 3950;

// --- GLOBAL OBJECTS ---
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

    // 3. Initialize Sensors & Inputs
    pinMode(pirOutPin, INPUT); 
    pinMode(buttonPin, INPUT); 

    // 4. Initialize LEDs
    for (int i = 0; i < 3; i++) {
        pinMode(ledPins[i], OUTPUT);
    }

    // 5. Initialize State Machine
    // Start in IdleState (Drone Inside)
    initialState = new IdleState(ledPins, myServo, lcd, PIN_ECHO, PIN_TRIG, sonar, pirOutPin, TEMP_PIN, BETA);
    stateManager = new StateManager(initialState);
    stateManager->init();
}

void loop()
{
    // Continuous update of the active state
    stateManager->update();
}