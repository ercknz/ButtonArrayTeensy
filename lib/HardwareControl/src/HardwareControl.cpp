#include "HardwareControl.h"
#include <Arduino.h>

HardwareControl::HardwareControl(){
}

void HardwareControl::SetupPins(){
    for(int i = 0; i < 6; i++){
        pinMode(_buttonLED_pins[i], OUTPUT);
        pinMode(_button_pins[i], INPUT_PULLUP);
    }
}

void HardwareControl::CheckForPress(bool duringExp){
    // Reads state of buttons
    for (int i = 0; i < 6; i++){
        buttonStates_M[i] = digitalRead(_button_pins[i]);
    }

    // Compares and registers a change in button state
    if (memcmp(buttonStates_M,lastButtonStates_M,sizeof(buttonStates_M)) != 0){
        for(int i = 0; i < 6; i++){
            if(lastButtonStates_M[i] != buttonStates_M[i]){
                buttonPressed_M = i;
                break;
            }
        }

        // Compares button to target
        TargetCheck(duringExp);
    }
}

void HardwareControl::CheckForGrip(bool duringExp){

}

byte * HardwareControl::GetButtonStates(){
    return buttonStates_M;
}

byte * HardwareControl::GetSwitchStates(){

}

void HardwareControl::SetLastButtonStates(){
    for(int i = 0; i < 6; i++){
        lastButtonStates_M[i] = buttonStates_M[i];
    }
}

void HardwareControl::SetLastSwitchStates(){

}

void HardwareControl::SetButtonLEDstates(byte * LEDstates){
    for (int i = 0; i < 6; i++){
        ButtonLEDstates_M[i] = LEDstates[i];
        if (LEDstates[i]){
            digitalWrite(_buttonLED_pins[i], HIGH);
        } else {
            digitalWrite(_buttonLED_pins[i], LOW);
        }
    }
}

void HardwareControl::SetSwitchLEDstates(byte * LEDstates){

}

void HardwareControl::SetButtonTarget(byte * targetArray, byte targetNum){
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
    SetButtonLEDstates(LEDarray);
}

void HardwareControl::SetSwitchTarget(byte * targetArray, byte targetNum){
   
}

bool HardwareControl::IsReady(){
    return readyForNext_M && !targetAvailable_M;
}

void HardwareControl::TargetCheck(bool duringExp){
    byte num = buttonPressed_M;
    if (buttonStates_M[num] == HIGH){
        totalPresses_M++;
        if (num == currentTarget_M){
            if (num == 0){
                if(!correctButtonPressed){
                    correctButtonPressed = true;
                }
                digitalWrite(_buttonLED_pins[num],LOW);
                targetAvailable_M = false;
                readyForNext_M = true;
                cornerCounter_M = 0;
            } else {
                digitalWrite(_buttonLED_pins[num],LOW);
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
        SetButtonTarget(_resetButtonTarget, _resetButton);
    } else {
        SetButtonTarget(_standbyButtonTarget,_resetButton);
    }
}

bool HardwareControl::GetCorrectButton(){
    return correctButtonPressed;
}

bool HardwareControl::GetCorrectSwitch(){

}