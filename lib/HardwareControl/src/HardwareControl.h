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
        void CheckForPress(bool duringExp);
        void CheckForGrip(bool duringExp);
        byte * GetButtonStates();
        byte * GetSwitchStates();
        void SetLastButtonStates();
        void SetLastSwitchStates();
        void SetButtonTarget(byte * targetArray, byte targetNumber);
        void SetSwitchTarget(byte * targetArray, byte targetNumber);
        bool IsReady();
        void Waiting(bool duringExp);
        bool GetCorrectButton();
        bool GetCorrectSwitch();
    
    private:
        byte _resetButton = 0;
        byte _resetButtonTarget[6] = {0};
        byte _resetSwitchTarget[4] = {0};
        byte _standbyButtonTarget[6] = {1};
        byte _standbySwitchTarget[4] = {1};
        const int _button_pins[6]  = { 2, 3, 4, 5, 6, 7};
        const int _switch_pins[4]  = { 8, 9, 10, 11};
        const int _buttonLED_pins[6] = {23,22,21,20,19,18};
        const int _switchLED_pins[4] = { 17, 16, 15, 14};
        byte  ButtonLEDstates_M[6] = {0};
        byte  SwitchLEDstates_M[4] = {0};
        int   totalPresses_M       = 0;
        byte  buttonStates_M[6]    = {1};
        byte  switchStates_M[4]    = {1};
        byte  lastButtonStates_M[6] = {1};
        byte  lastSwitchStates_M[4] = {1};
        byte  buttonPressed_M;
        bool  targetAvailable_M = false;
        bool  readyForNext_M = true;
        bool  correctButtonPressed = false;
        byte  currentTarget_M;
        void  TargetCheck(bool duringExp);
        void  SetButtonLEDstates(byte * LEDstates);
        void  SetSwitchLEDstates(byte * LEDstates);
        int   cornerCounter_M = 0;

};

#endif //HARDWARE_CONTROL_H