/**
 * Physical Button Array Logic 
 * 5 targets and reset button with LEDs and push buttons.
 * 
 * Code by Erick Nunez
 * created 11/8/2021
 */
#include "Arduino.h"
#include "HardwareControl.h"
#include "SerialPackets.h"
#include "ExperimentLogic.h"

HardwareControl buttonArray = HardwareControl();
ExperimentLogic expLogic = ExperimentLogic();
SerialPackets   serialToPC = SerialPackets(&Serial);

void setup()
{
  buttonArray.SetupPins();
  while(!Serial){}
  delay(1000);
  buttonArray.Waiting(expLogic.GetExpRunning());
  delay(1000);
  while(!expLogic.GetExpRunning()){
    buttonArray.CheckForPress(expLogic.GetExpRunning());
    buttonArray.SetLastButtonStates();
    if(buttonArray.GetCorrectButton()){
      expLogic.SetExpRunning();
      serialToPC.SendStart();
    }
  }
}

void loop()
{
  unsigned long previousTime, currentTime;
  const int LOOPTIME = 8;

  previousTime = millis();
  while (!expLogic.GetExpCompletedStatus()){
    currentTime = millis();

    // Generate Target if Experiment is running and button array is ready.
    if(buttonArray.GetReady() && !expLogic.GetExpCompletedStatus()){
      if(expLogic.IsCornerTarget()){
        expLogic.GenerateCornerTarget();
        buttonArray.SetTarget(expLogic.GetDummyTargetArray(), expLogic.GetDummyTargetNum());
      } else {
        expLogic.GenerateTarget();
        buttonArray.SetTarget(expLogic.GetTargetArray(), expLogic.GetTargetNum());
      }
    }

    // Checks for presses while running.
    buttonArray.CheckForPress(expLogic.GetExpRunning() && !expLogic.GetExpCompletedStatus());
    buttonArray.SetLastButtonStates();

    // Wait for 8ms to send packets.
    if(currentTime - previousTime >= LOOPTIME){
      previousTime = currentTime;
      serialToPC.SendStates(expLogic.GetTargetArray(), buttonArray.GetButtonStates());
    }
  }

  // Check to see if experiment is done.
  while(expLogic.GetExpCompletedStatus()){
    if(!serialToPC.GetCompleteSent()){
      buttonArray.Waiting(!expLogic.GetExpCompletedStatus());
      serialToPC.SendComplete();
    }
  }
}