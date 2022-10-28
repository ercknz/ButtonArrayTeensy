/* 
 * Experiment logic class for physical button array.
 * The logic works as if there were 9 targets to choose from.
 * If the target is a corner target, there are two possible followup targets (adjacent corners).
 * 
 * The target array is set up the following way:
 * numOfReps = [1CW 2CW 3 4CW 5CW 1CCW 2CCW 3(dummy) 4CCW 5CCW]
 * 
 * Coded by Erick Nunez
 * Created 11/17/2021
 */

#ifndef EXPERIMENT_LOGIC_H
#define EXPERIMENT_LOGIC_H

#include <Arduino.h>

class ExperimentLogic{
    public:
        ExperimentLogic();
        byte * GetTargetArray();
        byte GetTargetNum();
        void GenerateTarget();
        bool GetExpRunning();
        void SetExpRunning();
        bool GetExpCompletedStatus();
        bool IsCornerTarget();
        void GenerateCornerTarget();
        byte * GetDummyTargetArray();
        byte GetDummyTargetNum();

    private:
        const byte numOfTargets_M = 10;
        const byte maxReps_M    = 1;
        const byte maxTarget_M = 10;
        const byte minTarget_M = 1;
        byte numOfReps_M[10] = {0};
        byte totalReps_M = 0;
        byte targetArray_M[5];
        byte LEDTarget_M;
        byte dummyTargetArray_M[5];
        byte dummyTarget_M;
        byte trueTarget_M;
        bool expStarted_M = false;
        bool expCompleted_M = false;
        bool cornerTarget_M = false;
        
};

#endif //EXPERIMENT_LOGIC_H