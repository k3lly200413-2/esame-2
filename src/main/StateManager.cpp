#include "HardwareSerial.h"
#include "Arduino.h"
#include "StateManagerHeader.h"
#include "GenericState.h"
#include "LandingState.h"

StateManager::StateManager(GenericState* startingState)
{
  currentState = startingState;
  generalState = NULL;
  timing = 0;
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
  // 1. Check logic for state transitions
  GenericState* nextState = currentState->update();
  
  if (nextState != NULL)
  {
    setState(nextState);
  }

  // 2. Periodic Data Reporting (Every 1000ms)
  if (timing == 0)
  {
    timing = millis();
  }
  else if (millis() - timing > 1000) 
  {
    // Read temperature via the current state context
    int val = currentState->getTemp();

    // Send formatted message for the Python Plotter: "TEMP <value>"
    String message = "TEMP " + String(val);
    Serial.println(message); 
    
    timing = millis();
  }
}

void StateManager::setState(GenericState* newState)
{
  if (currentState != nullptr)
  {
    currentState->exitState();
    
    // CRITICAL: Delete the old state object to free up memory (RAM)
    delete currentState; 
  }

  // Switch to the new state
  currentState = newState;

  if (currentState != nullptr)
  {
    currentState->enterState();
  }
}