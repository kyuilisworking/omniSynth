#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include <math.h>
#include "mbed.h"

class OutputHandler {
public:
    OutputHandler(int outputRate);
    void beginOutput();
    void endOutput();
    void setPitch(float pitch);
    
private:
    float pitchToVoltage(float pitch, float amp, float phase);
    void update();
    AnalogOut aout;
    Serial pc;

    Ticker ticker;
    float outputRate, outputPeriod;
    float currentPitch;
    float maxPitch, minPitch, maxVolt, minVolt, slope;
    
    int count;
};

#endif