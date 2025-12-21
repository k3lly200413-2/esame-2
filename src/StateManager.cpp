#include "StateManagerHeader.h"
#include "GenericState.h"
#include "LandingState.h"

StateManager::StateManager(
  GenericState* startingState
)
{
  currentState = startingState;
  generalState = NULL;
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
  GenericState* nextState = currentState->update();
  Serial.println("Update");
  if (nextState != NULL)
  {
    setState(nextState);
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
