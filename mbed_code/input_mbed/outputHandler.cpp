#include "outputHandler.h"

OutputHandler::OutputHandler(int outputRate) : aout(p18), pc(USBTX, USBRX) {
    this->outputRate = outputRate;
    this->outputPeriod = 1.0f / (float) outputRate;
    this->count = 0;
    this->maxPitch = 1500.0f;
    this->minPitch = 100.0f;
    this->maxVolt = 1.0f;
    this->minVolt = 0.2f;
    this->slope = (maxVolt - minVolt) / (maxPitch - minPitch);
    this->currentPitch = 0;
}

void OutputHandler::beginOutput() {
    ticker.attach(this, &OutputHandler::update, outputPeriod);
}

void OutputHandler::endOutput() {
    ticker.detach();   
}

void OutputHandler::update() {
    //aout = 0.8f;
    aout = currentPitch;
}

void OutputHandler::setPitch(float pitch) {
    if (pitch < minPitch || pitch > maxPitch) {
        currentPitch = 0;
    } else {
        currentPitch = slope * (pitch - minPitch) + minVolt;
    }
}
