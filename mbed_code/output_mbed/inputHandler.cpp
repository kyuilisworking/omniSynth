#include "inputHandler.h"

InputHandler::InputHandler(PinName ain, int sampleRate) : ain(ain), pc(USBTX, USBRX){
    this->sampleRate = sampleRate;
    this->minPitch = 100.0f;
    this->maxPitch = 1500.0f;
    this->minVolt = 0.2f;
    this->maxVolt = 1.0f;
    this->slope = (maxPitch - minPitch) / (maxVolt - minVolt);
    this->zeroCount = 0;
    this->currentPitch = 0;
}

void InputHandler::startSampling(void) {
    float samplePeriod = 1.0 / (float) sampleRate;
    sampler.attach(this, &InputHandler::sample, samplePeriod);
}

void InputHandler::endSampling(void) {
    sampler.detach();
}

void InputHandler::sample(void) {
    float newPitch = ain.read();
    //pc.printf("c1\n\r");
    
    if (newPitch < 0.15f) {
        /*
        zeroCount++;
        if (zeroCount == 3) {
            currentPitch = 0.0f;
            zeroCount = 0;
        }*/
        currentPitch = 0.0f;
    } else {
        currentPitch = slope * (newPitch - minVolt) + minPitch;
    }
    //pc.printf("%f\n\r", currentPitch);
    pitchHandler.insertNewPitch(currentPitch);
}

float InputHandler::getPitch() {
    return pitchHandler.getPitch();
}

InputHandler::~InputHandler(void) {
}
