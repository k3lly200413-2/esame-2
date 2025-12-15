#ifndef STATE_H
#define STATE_H
#include<Arduino.h>
#include<LiquidCrystal_I2C.h>
#include<Servo.h>

class GenericState
{
    private:
        int ledPins[3];
        LiquidCrystal_I2C &lcd;
        Servo &servoUsed;

    protected:

        void display();

        void writeOnDisplay(int cursorX, int cursorY, char *text)
        {
            lcd.setCursor(cursorX, cursorY);
            lcd.print(text);
        }
        
        void openMotor()
        {
            servoUsed.write(180);
        }
        
        void closeMotor()
        {
            servoUsed.write(0);
        }
        
        void changeLed(int ledIndex, int newLedState)
        {
            digitalWrite(ledPins[ledIndex], newLedState);
        }
        
        void turnOnAllLeds()
        {
            for (int i = 0; i < 3; i++)
            {
                digitalWrite(ledPins[i], HIGH);
            }
        }

        void turnOffAllLeds()
        {
            for (int i = 0; i < 3; i++)
            {
                digitalWrite(ledPins[i], LOW);
            }
        }
        
        void temperature();
        
        void sonar();
        
        void pir();
        
        void button();

    public:
        GenericState(
            int leds[3],
            Servo &servo,
            LiquidCrystal_I2C &lcd
        );
        void update();
        void enterState();
        void exitState();
};

#endif
