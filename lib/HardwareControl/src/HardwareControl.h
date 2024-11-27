/* 
 * Button logic class for physical button array.
 * 
 * Button R = Center target queue button
 * Button 1 = Lower Left Button
 * Button 2 = Lower Right Button
 * Button 3 = Center Indented Button
 * Button 4 = Upper Left Button
 * Button 5 = Upper Right Button
 * Switch 6 = Lower left switch
 * Switch 7 = Lower right switch
 * Switch 8 = Upper left switch
 * Switch 9 = Upper right switch
 * 
 * Target 1 = Lower Left button LED 
 * Target 2 = Lower Right button LED
 * Target 3 = Center button LED
 * Target 4 = Upper Left button LED
 * Target 5 = Upper Left button LED
 * Target 6 = Lower Left switch LED
 * Target 7 = Lower right switch LED
 * Target 8 = Upper left switch LED
 * Target 9 = Upper right switch LED
 * 
 * Coded by Erick Nunez
 * Created 11/9/2021
 * Updated 11/6/2024
 */

#ifndef HARDWARE_CONTROL_H
#define HARDWARE_CONTROL_H

#include <Arduino.h>

class HardwareControl{
    public:
        HardwareControl();
        void SetupPins();
        void CheckForTrigger(bool duringExp);
        byte * GetArrayStates();
        void SetLastArrayStates();
        void SetTarget(byte * targetArray, byte targetNumber);
        bool IsReady();
        void Waiting(bool duringExp);
        bool GetCorrectTarget();
        void SetMode(byte modeToSet);
    
    private:
        byte _resetButton = 0;
        byte _resetTargets[6] = {0};
        byte _standbyTargets[6] = {1,1,1,1,1,1};
        const int _setupButtonSwitchPins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        const int _setupLEDPins[10] = {23, 22, 21, 20, 19, 18, 17, 16, 15, 14};
        int _buttonModePins[6]  = { 2, 3, 4, 5, 6, 7};
        int _switchModePins[6]  = { 2, 8, 9, 5, 10, 11};
        int _buttonModeLED_pins[6] = {23, 22, 21, 20, 19, 18};
        int _switchModeLED_pins[6] = {23, 17, 16, 20, 15, 14};
        int   activeLED_pins[6];            // = _LED_pins
        int   activeTrigger_pins[6];        // = _button_pins
        byte  TargetLEDstates_M[6] = {0};   // = LEDstates_M
        int   totalTriggers_M       = 0;    // = totalPresses_M
        byte  arrayStates_M[6]    = {0};    // = buttonStates_M
        byte  lastArrayStates_M[6] = {0};   // = lastButtonStates_M
        byte  targetTriggered_M;            // = buttonPressed_M
        bool  targetAvailable_M = false;
        bool  readyForNext_M = true;
        bool  correctTriggered = false;     // = correctButtonPressed
        byte  currentTarget_M;
        void  TargetCheck(bool duringExp);
        void  SetLEDstates(byte * LEDstates);
        int   cornerCounter_M = 0;
        byte  arrayMode_M;

};

#endif //HARDWARE_CONTROL_H