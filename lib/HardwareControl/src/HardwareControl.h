/* 
 * Button logic class for physical button array.
 * 
 * Button 1 = Center target queue button
 * Button 2 = Lower Left Button
 * Button 3 = Lower Right Button
 * Button 4 = Center Indented Button
 * Button 5 = Upper Left Button
 * Button 6 = Upper Right Button
 * 
 * Target 1 = Lower Left LED
 * Target 2 = Lower Right LED
 * Target 3 = Center LED
 * Target 4 = Upper Left LED
 * Target 5 = Upper Left LED
 * 
 * Coded by Erick Nunez
 * Created 11/9/2021
 */

#ifndef HARDWARE_CONTROL_H
#define HARDWARE_CONTROL_H

#include <Arduino.h>

class HardwareControl{
    public:
        HardwareControl();
        void SetupPins();
        void CheckForPress(bool duringExp);
        byte * GetButtonStates();
        void SetLastButtonStates();
        void SetTarget(byte * targetArray, byte targetNumber);
        bool IsReady();
        void Waiting(bool duringExp);
        bool GetCorrectButton();
    
    private:
        byte _resetButton = 0;
        byte _resetTarget[6] = {0,0,0,0,0};
        byte _standbyTarget[6] = {1,1,1,1,1};
        const int _button_pins[6]  = { 2, 3, 4, 5, 6, 7};
        const int _LED_pins[6]     = {23,22,21,20,19,18};
        byte  LEDstates_M[6]        = {0};
        int   totalPresses_M       = 0;
        byte  buttonStates_M[6]     = {1};
        byte  lastButtonStates_M[6] = {1};
        byte  buttonPressed_M;
        bool  targetAvailable_M = false;
        bool  readyForNext_M = true;
        bool  correctButtonPressed = false;
        byte  currentTarget_M;
        void  TargetCheck(bool duringExp);
        void  SetLEDstates(byte * LEDstates);
        int  cornerCounter_M = 0;

};

#endif //HARDWARE_CONTROL_H