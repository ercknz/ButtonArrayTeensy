/**
 * Physical Button Array Logic 
 * 5 targets and reset button with LEDs and push buttons.
 * Adding 4 switches to test grasping
 * 
 * Code by Erick Nunez
 * created 11/8/2021
 * Updated 10/31/2024
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
  // Setup Pins
  buttonArray.SetupPins();
  while(!Serial){}
  delay(1000);
  // Waiting on comm
  buttonArray.Waiting(expLogic.GetExpRunning());
  delay(1000);
  // Wait for experiment setup packet
  while(!serialToPC.GoodSetupRX()){
    serialToPC.ReadPackets();
    delay(100);
  }
  // Set experiment parameters
  buttonArray.SetMode(serialToPC.GetRequestedMode());
  expLogic.SetMaxReps(serialToPC.GetRequestedReps());
  delay(1000);
  // Wait on Start
  while(!expLogic.GetExpRunning()){
    buttonArray.CheckForTrigger(expLogic.GetExpRunning());
    buttonArray.SetLastArrayStates();
    if(buttonArray.GetCorrectTarget()){
      // Trigger received, start streaming
      expLogic.SetExpRunning();
      serialToPC.SendStart();
    }
  }
  Serial.print(9);
}

void loop()
{
  unsigned long previousTime, currentTime;
  const int LOOPTIME = 8;

  previousTime = millis();
  // Main Loop
  while (!expLogic.GetExpCompletedStatus()){
    currentTime = millis();

    // Generate Target if Experiment is running and button array is ready.
    if(buttonArray.IsReady() && !expLogic.GetExpCompletedStatus()){
      if(expLogic.IsCornerTarget()){
        expLogic.GenerateCornerTarget();
        buttonArray.SetTarget(expLogic.GetDummyTargetArray(), expLogic.GetDummyTargetNum());
      } else {
        expLogic.GenerateTarget();
        buttonArray.SetTarget(expLogic.GetTargetArray(), expLogic.GetTargetNum());
      }
    }

    // Checks for presses while running.
    buttonArray.CheckForTrigger(expLogic.GetExpRunning() && !expLogic.GetExpCompletedStatus());
    buttonArray.SetLastArrayStates();

    // Wait for 8ms to send packets.
    if(currentTime - previousTime >= LOOPTIME){
      previousTime = currentTime;
      serialToPC.SendStates(expLogic.GetTargetArray(), buttonArray.GetArrayStates());
    }
  }
  
  // Check to see if experiment is done.
  while(expLogic.GetExpCompletedStatus()){
    if(!serialToPC.GetCompleteSent()){
      buttonArray.Waiting(!expLogic.GetExpCompletedStatus());
      serialToPC.SendComplete();
    }
    delay(5000);
  }
}