#include "StateManagerHeader.h"
#include "GenericState.h"

StateManager::StateManager(
  GenericState* startingState,
  int servo,
  int pir,
  int leds[3],
  int button,
  int tempSensor,
  int sonar,
  uint8_t analog_pin)
{

  currentState = startingState;

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
  if (currentState != nullptr)
  {
    currentState->enterState();
  }
}

void StateManager::update()
{
  if (currentState != nullptr)
  {
    currentState->update();
  }
}

void StateManager::setState(GenericState* newState)
{
  if (currentState != nullptr)
  {
    currentState->exitState();
    currentState = newState;
    currentState->enterState();
  }
}
