/*
 * INPUT
 */


#include "mbed.h"
#include "micHandler.h"
#include "outputHandler.h"
#include "pitchDetector.h"
#include <string>

uint32_t SAMPLE_LENGTH = 512;
uint32_t WIDTH = 1;
uint32_t SAMPLE_RATE = 44100;
uint32_t OUTPUT_RATE = 40;

Serial pc(USBTX, USBRX);
MicHandler micHandler(p20, SAMPLE_LENGTH * WIDTH, SAMPLE_RATE);
OutputHandler outputHandler(OUTPUT_RATE);
PitchDetector pitchDetector("autocorrelation");

int main() {
    
    
    Wave* rawWave = NULL;
    micHandler.startSampling();
    outputHandler.beginOutput();
    
    while(1) {
        rawWave = micHandler.getSignal();

        if (rawWave != NULL) {
            float* signal = rawWave->getData();
            float freq = pitchDetector.detectPitch(*rawWave, SAMPLE_RATE, 1200);
            outputHandler.setPitch(freq);
            pc.printf("%f\n\r", freq);
            micHandler.clear();
            rawWave = NULL;
        }
    }
}
