#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <Arduino.h>
#include "GenericState.h"

class StateManager {
  private:

    /**
     * Need to put a pointer instead of a instance of a class becasue if I put the instance of it memory gets allocated only to that one variable 
    */
    GenericState* currentState;
    int servoPin;
    int pirPin;
    int ledPins[3];
    int buttonPin;
    int tempSensorPin;
    int sonarPin;
  
  public:
    StateManager(
      GenericState* startingState,
      int servoPin, 
      int pirPin, 
      int ledPins[3],
      int buttonPin,
      int tempSensorPin,
      int sonarPin,
      uint8_t analog_pin);

    void init();

    void update();

    void setState(GenericState* newState);
};

#endif
