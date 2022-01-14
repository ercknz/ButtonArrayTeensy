#include "ExperimentLogic.h"

ExperimentLogic::ExperimentLogic(){
}

void ExperimentLogic::SetExpRunning(){
    expStarted_M = true;
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
    return currentTarget_M;
}

void ExperimentLogic::GenerateTarget(){
    for (int i = 0; i < 5; i++){
        targetArray_M[i] = 0;
    }
    if (totalReps_M == (maxReps_M * numOfTargets_M)){
        expCompleted_M = true;
    }
    while(totalReps_M < (maxReps_M * numOfTargets_M)){
        srand(analogRead(A0));
        int targetNum = rand() % (maxTarget_M - minTarget_M + 1) + minTarget_M;
        if(numOfReps_M[targetNum-1] < maxReps_M){
            targetArray_M[targetNum - 1] = 1;
            numOfReps_M[targetNum-1] += 1;
            totalReps_M += 1;
            currentTarget_M = targetNum;
            break;
        }   
    }
}