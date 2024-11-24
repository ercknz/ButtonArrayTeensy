#include "ExperimentLogic.h"

ExperimentLogic::ExperimentLogic(){
    // Fill in dummy target
    numOfReps_M[7] = maxReps_M;
    totalReps_M = maxReps_M;
}

void ExperimentLogic::SetExpRunning(){
    expStarted_M = true;
}

void ExperimentLogic::SetMaxReps(byte numberOfReps){
    if (maxReps_M != numberOfReps){
        maxReps_M = numberOfReps;
        // Modify reps for experiment
        numOfReps_M[7] = numberOfReps;
        totalReps_M = numberOfReps;
    }
}

bool ExperimentLogic::GetExpRunning(){
    return expStarted_M;
}

bool ExperimentLogic::GetExpCompletedStatus(){
    return expCompleted_M;
}

byte * ExperimentLogic::GetTargetArray(){
    return targetArray_M;
}

byte ExperimentLogic::GetTargetNum(){
    return LEDTarget_M;
}

byte * ExperimentLogic::GetDummyTargetArray(){
    return dummyTargetArray_M;
}

byte ExperimentLogic::GetDummyTargetNum(){
    return dummyTarget_M;
}

void ExperimentLogic::GenerateTarget(){
    // Resets all targets
    for (int i = 0; i < 5; i++){
        targetArray_M[i] = 0;
    }
    cornerTarget_M = false;

    // Checks to see if experiment is completed
    if (totalReps_M == (maxReps_M * numOfTargets_M)){
        expCompleted_M = true;
    }

    // Generates new target
    while(totalReps_M < (maxReps_M * numOfTargets_M)){

        // Seeds new value and generates random target
        srand(analogRead(A0));
        int targetNum = rand() % (maxTarget_M - minTarget_M + 1) + minTarget_M;

        // Checks to see if all reps for that target are completed
        if(numOfReps_M[targetNum-1] < maxReps_M){
            LEDTarget_M = (targetNum > 5) ? targetNum - 5 : targetNum;
            targetArray_M[LEDTarget_M - 1] = 1;
            numOfReps_M[targetNum-1] += 1;
            totalReps_M += 1;
            trueTarget_M = targetNum;
            if (targetNum != 3) cornerTarget_M = true; // Center target which is not a corner target
            break;
        }   
    }
}

bool ExperimentLogic::IsCornerTarget(){
    return cornerTarget_M;
}

void ExperimentLogic::GenerateCornerTarget(){
    // depending on current target, generates next target
    switch (trueTarget_M){
        case 1:
        {
            dummyTarget_M = 4;
            break;
        }
        case 2:
        {
            dummyTarget_M = 1;
            break;
        }
        case 4:
        {
            dummyTarget_M = 5;
            break;
        }
        case 5:
        {
            dummyTarget_M = 2;
            break;
        }
        case 6:
        {
            dummyTarget_M = 2;
            break;
        }
        case 7:
        {
            dummyTarget_M = 5;
            break;
        }
        case 9:
        {
            dummyTarget_M = 1;
            break;
        }
        case 10:
        {
            dummyTarget_M = 4;
            break;
        }
    }
    for (int i = 0; i < 5; i++){
        dummyTargetArray_M[i] = 0;
    }
    dummyTargetArray_M[dummyTarget_M - 1] = 1;
    cornerTarget_M = false;
}