#ifndef STATE_H
#define STATE_H
#include<Arduino.h>
#include<LiquidCrystal_I2C.h>
#include<Servo.h>
#include<NewPing.h>

class GenericState
{
    protected:
        
        int ledPins[3];
        LiquidCrystal_I2C &lcd;
        Servo &servoUsed;
        int trig_pin;
        int echo_pin;
        NewPing &sonar;
        int pirState;
        uint8_t analog_pin;
        int beta;

        void writeOnDisplay(int cursorX, int cursorY, char *text);
        
        void openMotor();
        // {
        //     servoUsed.write(180);
        // }
        
        void closeMotor();
        // {
        //     servoUsed.write(0);
        // }
        
        void changeLed(int ledIndex);
        // {
        //     digitalWrite(ledPins[ledIndex], newLedState);
        // }
        
        void turnOnAllLeds();
        // {
        //     for (int i = 0; i < 3; i++)
        //     {
        //         digitalWrite(ledPins[i], HIGH);
        //     }
        // }

        void turnOffAllLeds();
        // {
        //     for (int i = 0; i < 3; i++)
        //     {
        //         digitalWrite(ledPins[i], LOW);
        //     }
        // }

        unsigned long getDistance();

        float getTemp();

    public:
        GenericState(
            int leds[3],
            Servo &servo,
            LiquidCrystal_I2C &lcdRef,
            int pin_echo,
            int pin_trig,
            NewPing &sonarUsed,
            int pirState,
            uint8_t analog_pin,
            int beta
        );
        // : servoUsed(servo), lcd(lcdRef) // Initializer list is required for References
        // {
        //     for(int i=0; i<3; i++) {
        //         ledPins[i] = leds[i];
        //     }
        // }

        // Destructor
        // Used when the class is no longer used
        virtual ~GenericState() {}
        
        virtual GenericState* update() = 0;
        
        virtual void enterState() = 0;
        
        virtual void exitState() = 0;
};

#endif
