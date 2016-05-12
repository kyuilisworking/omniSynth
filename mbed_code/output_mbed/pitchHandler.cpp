#include "pitchHandler.h"

CustomQueue::CustomQueue() {
    currentSize = 0;
    maxSize = 4;
}

void CustomQueue::push(float newVal) {
    if (currentSize < maxSize) {
        //shift the values
        for (int i = currentSize; i > 0; i--) {
            history[i] = history[i - 1];   
        }
        history[0] = newVal;
        currentSize++;
    } else {
        //shift the values
        for (int i = maxSize - 1; i >  0; i--) {
            history[i] = history[i - 1];
        }
        history[0] = newVal;
    }
}

void CustomQueue::pop() {
    if (currentSize > 0) {
        history[currentSize - 1] = 0;
    }
}

float CustomQueue::getAverage() {
    if (currentSize > 0) {
        float sum = 0;
        for (int i = 0; i < currentSize; i++) {
            sum += history[i];
        }
        return sum / currentSize;   
    } else {
        return 0;   
    }
}

int CustomQueue::getSize() {
    return currentSize;   
}

int CustomQueue::getMaxSize() {
    return maxSize;   
}

void CustomQueue::clear() {
    currentSize = 0;   
}

PitchHandler::PitchHandler() {
    pitchEpsilon = 8;
    pitchHistory = new CustomQueue();
    pitchOutlier = new CustomQueue();
}

PitchHandler::~PitchHandler() {
    delete pitchHistory;
    delete pitchOutlier;   
}

float autotune(float pitch) {
    //float autotunePitches [36] = {220, 233, 247, 262, 278, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 699, 740, 784, 831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1475, 1568, 1661};     
    float autotunePitches [22] = {220, 247, 262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 699, 784, 880, 998, 1047, 1175, 1319, 1397, 1568, 1760};
    float autotuneRanges [22] = {214, 233.5, 254.5, 278, 312, 339.5, 360.5, 416, 467, 508.5, 555, 623, 679, 741.5, 832, 939, 1022.5, 1111, 1247, 1358, 1482.5, 1664};
    
    int pitchIndex = 0;
    
    if (pitch >= 214) {
        while (pitch > autotuneRanges[pitchIndex]) {
            pitchIndex++;   
        }
        pitchIndex--;
        return autotunePitches[pitchIndex--];
    } else {
        return 0;   
    }
}

float PitchHandler::getPitch() {
    float average = pitchHistory->getAverage();
    return autotune(average);
}

void PitchHandler::insertNewPitch(float pitch) {
    float currentPitchAverage = pitchHistory->getAverage();
    if (abs(currentPitchAverage - pitch) < pitchEpsilon) {
        //check if deviation from current average is less than epsilon
        pitchHistory->push(pitch);
        pitchOutlier->pop();
    } else {
        //deviation greater than or equal to epsilon, add to outlier
        float pitchOutlierAverage = pitchOutlier->getAverage();
        if (abs(pitchOutlierAverage - pitch) < pitchEpsilon) {
            pitchOutlier->push(pitch);
            if (pitchOutlier->getSize() == pitchOutlier->getMaxSize()) {
                //swapping outlier and history
                CustomQueue* temp = pitchHistory;
                pitchHistory = pitchOutlier;
                pitchOutlier = temp;
                pitchOutlier->clear();
            }          
        } else {
            pitchOutlier->pop();
            pitchOutlier->push(pitch);
        }
    }
}