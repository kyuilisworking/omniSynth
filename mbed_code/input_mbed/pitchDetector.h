#ifndef PITCHDETECTOR_H
#define PITCHDETECTOR_H

#include "mbed.h"
#include "filters.h"
#include "wave.h"
#include "dsp.h"
#include <string>

class PitchDetector {
public:
    PitchDetector(string detectionMethod);
    float detectPitch(Wave& wave, int samplingRate, int filterFreq);

private:
    Serial pc;
    string detectionMethod;

    float autocorrelate(Wave& wave, int shift);
    float detectPitchAutocorrelation(Wave& wave, int samplingRate, int maxWidth);
        
};

#endif
