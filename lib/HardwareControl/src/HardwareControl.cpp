#include "HardwareControl.h"
#include <Arduino.h>

HardwareControl::HardwareControl(){
}

void HardwareControl::SetupPins(){
    for(int i = 0; i < 10; i++){
        pinMode(_setupLEDPins[i], OUTPUT);
        pinMode(_setupButtonSwitchPins[i], INPUT_PULLUP);
    }
}

void HardwareControl::CheckForTrigger(bool duringExp){
    // Reads state of buttons or switches
    for (int i = 0; i < 6; i++){
        arrayStates_M[i] = digitalRead(activeTrigger_pins[i]);
    }

    // Compares and registers a change in button or switch states
    if (memcmp(arrayStates_M, lastArrayStates_M, sizeof(arrayStates_M)) != 0){
        for(int i = 0; i < 6; i++){
            if(lastArrayStates_M[i] != arrayStates_M[i]){
                targetTriggered_M = i;
                //Serial.print(i);
                break;
            }
        }

        // Compares button to target
        TargetCheck(duringExp);
    }
}

byte * HardwareControl::GetArrayStates(){
    return arrayStates_M;
}

void HardwareControl::SetLastArrayStates(){
    for(int i = 0; i < 6; i++){
        lastArrayStates_M[i] = arrayStates_M[i];
    }
}

void HardwareControl::SetLEDstates(byte * LEDstates){
    for (int i = 0; i < 6; i++){
        TargetLEDstates_M[i] = LEDstates[i];
        if (LEDstates[i]){
            digitalWrite(activeLED_pins[i], HIGH);
        } else {
            digitalWrite(activeLED_pins[i], LOW);
        }
    }
}

void HardwareControl::SetTarget(byte * targetArray, byte targetNum){
    if (targetNum != 3) cornerCounter_M += 1;
    targetAvailable_M = true;
    currentTarget_M = targetNum;
    byte LEDarray[6];
    for(int i = 0; i < 6; i++){
        if (i == 0){
            LEDarray[i] = 0;
        } else {
            LEDarray[i] = targetArray[i-1];
        }
    }
    if (!targetNum){
        LEDarray[0] = 1;
    }
    SetLEDstates(LEDarray);
}

bool HardwareControl::IsReady(){
    return readyForNext_M && !targetAvailable_M;
}

void HardwareControl::TargetCheck(bool duringExp){
    byte num = targetTriggered_M;
    if (arrayStates_M[num] == HIGH){
        totalTriggers_M++;
        if (num == currentTarget_M){
            if (num == 0){
                if(!correctTriggered){
                    correctTriggered = true;
                }
                digitalWrite(activeLED_pins[num],LOW);
                targetAvailable_M = false;
                readyForNext_M = true;
                cornerCounter_M = 0;
            } else {
                digitalWrite(activeLED_pins[num],LOW);
                if (cornerCounter_M == 1){
                    targetAvailable_M = false;
                    readyForNext_M = true;
                } else {
                    cornerCounter_M = 0;
                    Waiting(duringExp);
                }
            }
        } 
    } 
    delay(5); // Delay a little bit to avoid bouncing
}

void HardwareControl::Waiting(bool duringExp){
    if(duringExp){
        SetTarget(_resetTargets, _resetButton);
    } else {
        SetTarget(_standbyTargets, _resetButton);
    }
}

bool HardwareControl::GetCorrectTarget(){
    return correctTriggered;
}

void HardwareControl::SetMode(byte modeToSet){
    arrayMode_M = modeToSet;
    for (int i = 0; i < 6; i++){
        switch (modeToSet)
        {
        case 1:
            // Buttons Only Mode
            activeLED_pins[i] = _buttonModeLED_pins[i];
            activeTrigger_pins[i] = _buttonModePins[i];
            break;

        case 2:
            // Switches and Center button Mode
            activeLED_pins[i] = _switchModeLED_pins[i];
            activeTrigger_pins[i] = _switchModePins[i];
            break;
        
        default:
            break;
        }
    }
    Serial.print(modeToSet);
}