#include "pitchDetector.h"

PitchDetector::PitchDetector(string detectionMethod) :  detectionMethod(detectionMethod), 
                                                        pc(USBTX, USBRX)
{
}

/*
 * Returns the estimation of the fundamental frequency
 */
float PitchDetector::detectPitch(Wave& wave, int samplingRate, int filterFreq) {
    if (detectionMethod.compare("autocorrelation") == 0) {
        return PitchDetector::detectPitchAutocorrelation(wave, samplingRate, 500);
    } else {
        return 0;
    }
}

/*
 * Finds the autocorrelation coefficient for the data with specified shift
 */
float PitchDetector::autocorrelate(Wave& wave, int shift) {
    int n = wave.getDataSize();
    float autocoeff = 0;
    float* data = wave.getData();

    for (int i = 0; i < n - shift; i++) {
        autocoeff += data[i] * data[i + shift];
    }
    return autocoeff / 10000.f;
}

/*
 * Detects the fundamental frequency using the autocorrelation function
 */
float PitchDetector::detectPitchAutocorrelation(Wave& wave, int samplingRate, int maxWidth) {
    Filters::lowPass(wave, samplingRate, 1200);

    int n = wave.getDataSize();
    int currentShift = 10;
    bool pitchDetected = false;
    //initial previousCoeff to reduce load in the loop (no if statement)
    float previousCoeff = PitchDetector::autocorrelate(wave, currentShift);
    float previousDerivative = -1;
    float autocoeff, currentDerivative;
    currentShift++;

    while (!pitchDetected && (currentShift < maxWidth)) {
        autocoeff = PitchDetector::autocorrelate(wave, currentShift);
        currentDerivative = autocoeff - previousCoeff;
        if (currentDerivative < 0 && previousDerivative >= 0) {
            //peak found
            pitchDetected = true;
        } else {
            previousDerivative = currentDerivative;
            previousCoeff = autocoeff;
            currentShift += 5;
        }
    }
    
    if (pitchDetected) {
        //fine tuning the pitch
        pitchDetected = false;
        previousDerivative = currentDerivative;
        previousCoeff = autocoeff;
        currentShift -= 1;
        
        while (!pitchDetected) {
            autocoeff = PitchDetector::autocorrelate(wave, currentShift);
            currentDerivative = previousCoeff - autocoeff;
            if (currentDerivative >= 0 && previousDerivative < 0) {
                pitchDetected = true;   
            } else {
                previousDerivative = currentDerivative;
                previousCoeff = autocoeff;
                currentShift -= 1; 
            }
        } 
        
        currentShift += 2;
    }

    if (!pitchDetected) {
        return 0;
    } else {
        //get the first local maximum so need to divide by 2
        return (float) samplingRate / currentShift / 2;
    }
}
