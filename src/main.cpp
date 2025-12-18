#include<Servo.h>
#include <Arduino.h>
#include<LiquidCrystal_I2C.h>
#include "GenericState.h"
#include "DroneInsideState.h"
#include "TakeOffState.h"

// DO NOT USE SEMICOLONS THEY BECOME PART OF THE MACRO 
// E.G. Don't do:
    // #define I2C_ADDRESS 0x27;
    // but do:
#define I2C_ADDRESS 0x27
#define DISPLAY_ROW 2
#define DISPLAY_COLUMN 16
#define PIN_TRIG 8
#define PIN_ECHO 7
#define SERVO_PIN 3

Servo myServo;

int pirOutPin = 2;
int val = LOW;
int pirState = LOW;
const float BETA = 3950;
int ledPins[] = { 4, 5, 6 };
int buttonPin = 9;

int ledPin = 13;



LiquidCrystal_I2C lcd(I2C_ADDRESS, 16, 2);

GenericState* initialState;

void setup()
{
    myServo.attach(SERVO_PIN);
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();

    pinMode(2, INPUT);  // PIR
    pinMode(6, INPUT);  // BUTTON

    // LEDs
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);

    initialState = new TakeOffState(ledPins, myServo, lcd, PIN_ECHO, PIN_TRIG);
    // StateManager* stateManager = new StateManager(initialState);
    initialState->enterState();
}

void loop()
{
    // Serial.println("We're Looping");

    initialState->update();
}
