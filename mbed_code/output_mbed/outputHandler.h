#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include <string>
#include <math.h>
#include <vector>
#include "mbed.h"
#include "pitchHandler.h"
#include "frequencySpectrumGenerator.h"

class OutputHandler {
public:
    OutputHandler(string instrument, float samplingRate);
    void beginOutput();
    void setPitch(float pitch);
    void setToWhistleMode();
    void rotateInstrument();
    void setStaccatoMode();
    void setVibratoMode();
    
private:
    void update();
    void calculateWave(float pitch);
    void calculateFadeWave();
    void calculateVibrato();
    
    AnalogOut aout;
    Serial pc;
    Ticker ticker;
    FrequencySpectrumGenerator freqSpecGen;
    //PitchHandler pitchHandler;
    
    float pitch;

    string instrument;
    float samplingRate;
    int samplePeriod;

    float dPhase;
    float phase;
    
    int currentWave;
    float wave [2][550];
    //float fadeOutWave[101];
    volatile int waveIndex; //the current index of the wave array
    int waveSize; //the size of the wave ->not necessarily 30
    
    bool whistleMode;
    bool vibratoMode;
    bool staccatoMode;
    
    float fadeWave[2][51];
    int fadeInOut; //0 is in, 1 is out
    int fadeWaveSize;
    int fadeWaveCount;
    int fadeWaveScale;
    int fadeWaveScaleIndex;
    int fadeInCountMax;
    int fadeOutCountMax;
    int fadeCount;
    bool pitchSetZero;
    
    int vibrato[100];
    int vibratoSize;
    int vibratoIndex;
    int vibratoUpdateMax;
    int vibratoUpdateCount;
};

#endif