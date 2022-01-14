/* 
 * Experiment logic class for physical button array.
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

    private:
        const byte numOfTargets_M = 5;
        const byte maxReps_M    = 10;
        const byte maxTarget_M = 5;
        const byte minTarget_M = 1;
        byte numOfReps_M[5] = {0};
        byte totalReps_M = 0;
        byte targetArray_M[5];
        byte currentTarget_M;
        bool expStarted_M = false;
        bool expCompleted_M = false;
        
};

#endif //EXPERIMENT_LOGIC_H