#include "StateManagerHeader.h"
#include "GenericState.h"

StateManager::StateManager(
  GenericState* startingState,
  int servo,
  int pir,
  int leds[3],
  int button,
  int tempSensor,
  int sonar)
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
  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(tempSensorPin, INPUT);
  pinMode(sonarPin, INPUT);

  for (int i=0; i < 3; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }

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
