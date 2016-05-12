#include "micHandler.h"

MicHandler::MicHandler(PinName ain, int sampleLength, int sampleRate) : ain(ain), pc(USBTX, USBRX){
    this->sampleLength = sampleLength;
    this->sampleRate = sampleRate;
    dataIndex = 0;
    signal = new float[sampleLength];
    wave = new Wave(signal, sampleLength);
}

void MicHandler::startSampling(void) {
    int samplePeriod = 1.0 / (float) sampleRate * 1000000;
    sampler.attach_us(this, &MicHandler::sample, samplePeriod);
}

void MicHandler::endSampling(void) {
    sampler.detach();
}

void MicHandler::clear(void) {
    dataIndex = 0;
}

Wave* MicHandler::getSignal(void) {
    if (dataIndex == sampleLength) {
        return wave;
    } else {
        return NULL;
    }
}

void MicHandler::sample(void) {
    
    if (dataIndex < sampleLength) {
        signal[dataIndex] = ain.read();
        dataIndex += 1;
    }
    
}

MicHandler::~MicHandler(void) {
    delete wave;   
}
