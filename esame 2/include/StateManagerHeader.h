#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <Arduino.h>

enum State {
  INSIDE,
  OUTSIDE
};

class StateManager {
  private:
    State currentState;
    int servoPin;
    int pirPin;
    int ledPins[3];
    int buttonPin;
    int tempSensorPin;
    int sonarPin;
  
  public:
    StateManager(
      int servoPin, 
      int pirPin, 
      int ledPins[3],
      int buttonPin,
      int tempSensorPin,
      int sonarPin);

    void init();

    void update();

    void setState(State newState);
};

#endif
