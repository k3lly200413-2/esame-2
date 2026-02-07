#include <Servo.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "GenericState.h"
#include "DroneInsideState.h"
#include "TakeOffState.h"
#include "LandingState.h"
#include "IdleState.h"
#include "StateManagerHeader.h" 

// PIN Definitions
#define I2C_ADDRESS 0x27
#define PIN_TRIG 8
#define PIN_ECHO 7
#define SERVO_PIN 3 
#define TEMP_PIN 13
#define PIR_OUT_PIN 12
#define BUTTON_PIN 9

int ledPins[] = { 4, 5, 6 };

const float BETA = 3950;

// Static memory allocation to avoid allocating new memory and delete meory every state change
// this is to avoid heap fragmentation

Servo myServo;
LiquidCrystal_I2C lcd(I2C_ADDRESS, 16, 2);
NewPing sonar(PIN_TRIG, PIN_ECHO);

// Pointer definition to states to be used throughout main file

GenericState* initialState;
StateManager* stateManager;

void setup()
{
    // Setting up needed controlled elements

    Serial.begin(9600);
    
    // Display
    lcd.init();
    lcd.backlight();

    // Servo
    myServo.attach(SERVO_PIN);

    // Sensors
    pinMode(PIR_OUT_PIN, INPUT); 
    pinMode(PIR_OUT_PIN, INPUT); 

    // LEDs
    for (int i = 0; i < 3; i++) {
        pinMode(ledPins[i], OUTPUT);
    }

    // State Machine
    initialState = new IdleState(ledPins, myServo, lcd, PIN_ECHO, PIN_TRIG, sonar, PIR_OUT_PIN, TEMP_PIN, BETA);
    stateManager = new StateManager(initialState);
    stateManager->init();
}

void loop()
{
    stateManager->update();
}