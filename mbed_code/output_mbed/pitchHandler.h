#ifndef PITCHHANDLER_H
#define PITCHHANDLER_H

#include <math.h>


class CustomQueue {
    //used for optimization since dynamic allocation should be slower
    public:
        CustomQueue();
        float getAverage();
        void push(float newVal);
        void pop();
        int getSize();
        int getMaxSize();
        void clear();
    private:
        float history [4];
        int currentSize;
        int maxSize;
};

class PitchHandler {
    public:
        PitchHandler();
        ~PitchHandler();
        void insertNewPitch(float pitch);
        float getPitch();
        
    private:
        CustomQueue* pitchHistory;
        CustomQueue* pitchOutlier;
      
        float pitchEpsilon;
};

#endif