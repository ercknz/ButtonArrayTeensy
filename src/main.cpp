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
  while(!serialToPC.DataAvailable()){}
  serialToPC.ReadPackets();
  expLogic.SetMaxReps(serialToPC.GetRequestedReps());
  buttonArray.SetMode(serialToPC.GetRequestedMode());
  delay(1000)
  while(!expLogic.GetExpRunning()){
    buttonArray.CheckForPress(expLogic.GetExpRunning());
    buttonArray.SetLastButtonStates();
    if(buttonArray.GetCorrectButton()){
      // Trigger received, start streaming
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
  // Main Loop
  while (!expLogic.GetExpCompletedStatus()){
    currentTime = millis();

    // Generate Target if Experiment is running and button array is ready.
    if(buttonArray.IsReady() && !expLogic.GetExpCompletedStatus()){
      if(expLogic.IsCornerTarget()){
        expLogic.GenerateCornerTarget();
        buttonArray.SetButtonTarget(expLogic.GetDummyTargetArray(), expLogic.GetDummyTargetNum());
      } else {
        expLogic.GenerateTarget();
        buttonArray.SetButtonTarget(expLogic.GetTargetArray(), expLogic.GetTargetNum());
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
    delay(5000);
  }
}