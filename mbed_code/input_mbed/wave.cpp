#include "wave.h"

Wave::Wave(float* data, int dataSize) {
    this->data = data;
    this->dataSize = dataSize;
}

Wave::~Wave() {
    delete data;
}

float* Wave::getData() {
    return data;
}

int Wave::getDataSize() {
    return dataSize;
}

void Wave::setData(float* data, int dataSize) {
    this->data = data;
    this->dataSize = dataSize;
}