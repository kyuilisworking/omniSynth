#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "mbed.h"
#include "pitchHandler.h"

class InputHandler {
public:
    InputHandler(PinName ain, int sampleRate);
    ~InputHandler();
    void startSampling(void);
    void endSampling(void);
    float getPitch();
    
private:
    Ticker sampler;
    AnalogIn ain;
    PitchHandler pitchHandler;
    Serial pc;
    
    int sampleRate;
    int dataIndex;
    int zeroCount;

    float currentPitch, minPitch, maxPitch, minVolt, maxVolt, slope;
    bool sampleReady;

    void sample(void);
    
};

#endif
