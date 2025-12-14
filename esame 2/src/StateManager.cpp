#include "StateManagerHeader.h"

StateManager::StateManager(
  int servo,
  int pir,
  int leds[3],
  int button,
  int tempSensor,
  int sonar)
{
  servoPin = servo;
  pirPin = pir;
  ledPins[0] = leds[0];
  ledPins[1] = leds[1];
  ledPins[2] = leds[2];
  buttonPin = button;
  tempSensorPin = tempSensor;
  sonarPin = sonar;
}

void StateManager::init()
{
  // TODO: I don't know what I have to do yet here
}

void StateManager::update()
{

}

void StateManager::setState(State newState)
{
  this->currentState = newState;
}
