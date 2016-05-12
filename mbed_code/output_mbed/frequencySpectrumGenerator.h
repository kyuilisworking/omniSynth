#ifndef FREQUENCYSPECTRUMGENERATOR_H
#define FREQUENCYSPECTRUMGENERATOR_H

#include "mbed.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <sstream>

/*
 * Set up by adding first highest order coefficient to lowest order coefficient
 */

class Predictor {
    public:
        Predictor();
        float predict(float pitch);
        void addCoefficient(float coefficient);
        void convertToFloatArray();
        
    private:
        std::vector<float> coefficients;
        int numberOfTerms;
        float* coefficients_f;
      
};

/*
 * Set up by adding first lowest harmonic to highest harmonic
 * File should be in the following format (in this example the predictors are 2nd order, although they can be any):
 *
 * 0.1, 0.4, 10.8
 * 0.4, 0.3, 10.7
 * .... 
 */

class FrequencySpectrumGenerator {
    public:
        FrequencySpectrumGenerator(string instrument);
        std::vector<float>& getHarmonicAmplitudes(float pitch);
        int getNumberOfHarmonics();
        void rotateInstrument();
        
    private:
        std::vector<Predictor> harmonicPredictors;
        std::vector<float> harmonicAmplitudes;
        Serial pc;
        InterruptIn _interrupt;
        int numberOfHarmonics;
        int instrumentIndex;
        string instruments[5];
        
        void readFile(string filename);
        
        
};

#endif