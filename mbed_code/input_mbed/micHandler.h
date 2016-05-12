#ifndef MICHANDLER_H
#define MICHANDLER_H

#include "mbed.h"
#include "wave.h"

class MicHandler {
public:
    MicHandler(PinName ain, int sampleLength, int sampleRate);
    ~MicHandler();
    void startSampling(void);
    void endSampling(void);
    void clear(void);
    Wave* getSignal(void);
private:
    Ticker sampler;
    AnalogIn ain;
    int sampleLength;
    int sampleRate;
    float* signal;
    Wave* wave;
    int dataIndex;

    void sample(void);
    
    Serial pc;
};
#endif
