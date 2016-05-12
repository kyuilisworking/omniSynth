#include "filters.h"
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

void Filters::lowPass(Wave& wave, int samplingRate, int cutoffFreq) {
    float* data = wave.getData();
    float dT = 1.0f / (float)samplingRate;
    float RC = 1.0f / (2.0 * M_PI * (float)cutoffFreq);
    float alpha = dT / (RC + dT);

    int n = wave.getDataSize();

    data[0] = 0;

    for (int i = 1; i < n; i++) {
        data[i] = alpha * data[i] + (1.0f - alpha) * data[i - 1];
    }

    float dT1 = data[1] - data[2];
    data[0] = data[1] + dT1;
}