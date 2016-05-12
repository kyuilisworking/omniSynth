#include "outputHandler.h"
#ifndef M_PI
    #define M_PI 3.14159
#endif

OutputHandler::OutputHandler(string instrument, float samplingRate) : instrument(instrument), aout(p18), pc(USBTX, USBRX), freqSpecGen(instrument) {
    this->pitch = 0;
    this->samplingRate = samplingRate;
    this->samplePeriod = 1.0f / (float) samplingRate * 1000000;
    this->dPhase = 2 * M_PI * pitch / samplingRate;
    this->waveIndex = 0;
    this->waveSize = 100;
    this->phase = 0;
    this->currentWave = 0;
    
    this->whistleMode = false;
    this->vibratoMode = false;
    this->staccatoMode = false;
    
    this->fadeWaveSize = 50;
    this->fadeWaveCount = 0;
    this->fadeWaveScale = 3;
    this->fadeWaveScaleIndex = 0;
    this->fadeInOut = 0;
    this->fadeInCountMax = 100;
    this->fadeOutCountMax = 100;
    this->fadeCount = 0;
    this->pitchSetZero = false;
    
    this->vibratoSize = 100;
    this->vibratoIndex = 0;
    this->vibratoUpdateMax = 40;
    this->vibratoUpdateCount = 0;
    
    calculateFadeWave();
    calculateVibrato();
}

void OutputHandler::setPitch(float pitch) {
    if (abs(this->pitch - pitch) > 0.5f) {
        //pc.printf("%f\n\r", pitch);
        if (pitch != 0) {
            this->pitch = pitch;
            calculateWave(pitch);
            fadeWaveCount = 0;
            fadeWaveScaleIndex = 0;
            fadeCount = 0;
            pitchSetZero = false;
            if (!staccatoMode) {
                fadeInOut = 0;
            }
        } else {
            this->pitch = 0;
            fadeWaveCount = 0;
            fadeWaveScaleIndex = 0;
            fadeCount = 0;
            fadeInOut = 1;
        }
    }
}


void OutputHandler::setToWhistleMode() {
    whistleMode = true;    
}

void OutputHandler::setStaccatoMode() {
    staccatoMode = !staccatoMode;
    fadeInOut = 1;
}

void OutputHandler::setVibratoMode() {
    vibratoMode = !vibratoMode;   
}

void OutputHandler::beginOutput() {
    ticker.attach_us(this, &OutputHandler::update, samplePeriod);
}

void OutputHandler::calculateWave(float pitch) {
    if (!whistleMode) {
        int newWaveSize;
        int currentIndex;
        if (currentWave == 0) {
            if (pitch == 0) {
                for (int i = 0; i < 50; i++) {
                    wave[1][i] = 0;   
                }
                newWaveSize = 50; 
            } else {
                int currentHarmonic = 1;
                std::vector<float> harmonicAmps = freqSpecGen.getHarmonicAmplitudes(pitch);
                int numOfHarmonics = freqSpecGen.getNumberOfHarmonics();
                
                for (std::vector<float>::iterator amp = harmonicAmps.begin() ; amp != harmonicAmps.end(); ++amp) {
                    if (currentHarmonic == 1) {
                        float phase = 0;
                        float dPhase = 2 * M_PI * (pitch) / samplingRate;
                        int newWaveIndex = 0;
                
                        while (phase < 2 * M_PI) {
                            //pc.printf("%f\n\r", phase);
                            /*
                            float val = (float) *amp * sin(phase - M_PI / 2) + (float) *amp;
                            if (val >= 0) {
                                if (val <= 1) {
                                    wave[1][newWaveIndex] = val;  
                                } else {
                                    wave[1][newWaveIndex] = 1;   
                                }
                            } else {
                                wave[1][newWaveIndex] = 0;   
                            }
                            */
                            wave[1][newWaveIndex] = (float) *amp * sin(phase - M_PI / 2) + (float) *amp;   
                            phase += dPhase;
                            newWaveIndex++;
                        }
                        newWaveSize = newWaveIndex;
                        currentHarmonic++;
                    } else {
                        float phase = 0;
                        float dPhase = 2 * M_PI * (pitch) * currentHarmonic / samplingRate;
                
                        for (int i = 0; i < newWaveSize; i++) {
                            /*
                            float val = (float) *amp * sin(phase - M_PI / 2) + (float) *amp;
                            if (val >= 0) {
                                if (val <= 1) {
                                    wave[1][i] = val;  
                                } else {
                                    wave[1][i] = 1;   
                                }
                            } else {
                                wave[1][i] = 0;   
                            }
                            */
                            wave[1][i] += (float)*amp * sin(phase - M_PI / 2) + (float) *amp;   
                            phase += dPhase;
                        }
                        currentHarmonic++;
                    }
                }
            }
            
            if (pitch == 0) {
                currentIndex = 0;
            } else {
                float currentVoltage = wave[0][waveIndex];
                for (int i = 0; i < newWaveSize; i++) {
                    if (abs(currentVoltage - wave[1][i]) < 0.02f) {
                        currentIndex = i;
                        break;   
                    }   
                }
            }

            waveSize = newWaveSize;
            waveIndex = currentIndex;
            currentWave = 1;
        } else { //if using one Mbed for output
            if (pitch == 0) {
                for (int i = 0; i < 50; i++) {
                    wave[0][i] = 0;   
                }
                newWaveSize = 50; 
            } else {
                int currentHarmonic = 1;
                std::vector<float> harmonicAmps = freqSpecGen.getHarmonicAmplitudes(pitch);
                int numOfHarmonics = freqSpecGen.getNumberOfHarmonics();
                
                for (std::vector<float>::iterator amp = harmonicAmps.begin() ; amp != harmonicAmps.end(); ++amp) {
                    if (currentHarmonic == 1) {
                        float phase = 0;
                        float dPhase = 2 * M_PI * (pitch) / samplingRate;
                        int newWaveIndex = 0;
                
                        while (phase < 2 * M_PI) {
                            /*
                            float val = (float) *amp * sin(phase - M_PI / 2) + (float) *amp;
                            if (val >= 0) {
                                if (val <= 1) {
                                    wave[0][newWaveIndex] = val;  
                                } else {
                                    wave[0][newWaveIndex] = 1;   
                                }
                            } else {
                                wave[0][newWaveIndex] = 0;   
                            }*/
                            
                            wave[0][newWaveIndex] = (float) *amp * sin(phase - M_PI / 2) + (float) *amp;   
                            phase += dPhase;
                            newWaveIndex++;
                        }
                        newWaveSize = newWaveIndex;
                        currentHarmonic++;
                    } else {
                        double phase = 0;
                        double dPhase = 2 * M_PI * (pitch) * currentHarmonic / samplingRate;
                
                        for (int i = 0; i < newWaveSize; i++) {
                            /*
                            float val = (float) *amp * sin(phase - M_PI / 2) + (float) *amp;
                            if (val >= 0) {
                                if (val <= 1) {
                                    wave[0][i] = val;  
                                } else {
                                    wave[0][i] = 1;   
                                }
                            } else {
                                wave[0][i] = 0;   
                            }*/
                            
                            wave[0][i] += (float) *amp * sin(phase - M_PI / 2) + (float) *amp;    
                            phase += dPhase;
                        }
                        currentHarmonic++;
                    }
                }
            }
            
            if (pitch == 0) {
                currentIndex = 0;
            } else {
                float currentVoltage = wave[1][waveIndex];
                for (int i = 0; i < newWaveSize; i++) {
                    if (abs(currentVoltage - wave[0][i]) < 0.02f) {
                        currentIndex = i;
                        break;   
                    }   
                }
            }
            waveSize = newWaveSize;
            waveIndex = currentIndex;
            currentWave = 0;
        } 
    } else {
        //pc.printf("check\n\r");
        if (currentWave == 0) {
            if (pitch == 0) {
                for (int i = 0; i < 50; i++) {
                    wave[1][i] = 0;   
                }
                waveSize = 50; 
                currentWave = 1;
                waveIndex = 0;
            } else {
                float phase = 0;
                float dPhase = 2 * M_PI * pitch / samplingRate;
                int newWaveIndex = 0;
                while (phase < 2 * M_PI) {
                    wave[1][newWaveIndex] = 0.5f * sin(phase - M_PI / 2) + 0.5f;
                    //wave[1][newWaveIndex] = 0.5f * sin(phase);
                    newWaveIndex++;
                    phase += dPhase;
                }
                if (abs(wave[currentWave][waveIndex] - 0) < 0.15f) {
                    waveIndex = 0;   
                    waveSize = newWaveIndex;
                } else {
                    float currentPhase = 2*M_PI * waveIndex / waveSize;
                    waveIndex = currentPhase * newWaveIndex / 2*M_PI;
                    waveSize = newWaveIndex;
                }
                currentWave = 1;       
            }
            
        } else {
            if (pitch == 0) {
                for (int i = 0; i < 50; i++) {
                    wave[0][i] = 0;   
                }
                waveSize = 50;
                waveIndex = 0;
                currentWave = 0;
            } else {
                float phase = 0;
                float dPhase = 2 * M_PI * pitch / samplingRate;
                int newWaveIndex = 0;
                while (phase < 2 * M_PI) {
                    wave[0][newWaveIndex] = 0.5f * sin(phase - M_PI / 2) + 0.5f;  
                    //wave[0][newWaveIndex] = 0.5f * sin(phase);
                    newWaveIndex++;
                    phase += dPhase;
                }
                if (abs(wave[currentWave][waveIndex] - 0) < 0.15f) {
                    waveIndex = 0;   
                    waveSize = newWaveIndex;
                } else {
                    float currentPhase = 2*M_PI * waveIndex / waveSize;
                    waveIndex = currentPhase * newWaveIndex / 2*M_PI;
                    waveSize = newWaveIndex;
                }
                currentWave = 0;    
            }        
        }
    }
}

void OutputHandler::calculateFadeWave() {
    //fade in
    for (int i = 0; i < fadeWaveSize + 1; i++) {
        fadeWave[0][i] = 1.0f / (1 + exp(-0.2f * (float)i + 0.2));
    }
    fadeWave[0][fadeWaveSize] = fadeWave[0][fadeWaveSize-1];
    
    //fade out
    for (int i = 0; i < fadeWaveSize; i++) {
        fadeWave[1][i] = fadeWave[0][fadeWaveSize] * 2.0f / (1 + exp(0.2f * (float)i));
        //fadeWave[1][i] =  1.0f / (1 + exp(0.1f * (float)i) - 0.2f);
    }
    fadeWave[1][fadeWaveSize] = 0;
}

void OutputHandler::calculateVibrato() {
    float numberInPeriod = 100;
    float phase = 0;
    float dPhase = 2 * M_PI / numberInPeriod;
    for (int i = 0; i < numberInPeriod; i++) {
        phase += dPhase;
        vibrato[i] = (int)((int)(1.8f * sin(phase)) + samplePeriod);   
    }
}


void OutputHandler::update() {
    aout = wave[currentWave][waveIndex] * fadeWave[fadeInOut][fadeWaveCount];;
    waveIndex++;
    fadeCount++;

    if (waveIndex >= waveSize) {
        waveIndex = 0;
    }
    
    if (fadeInOut == 0) { //fade in
        if (fadeCount > fadeInCountMax) {
            fadeCount = 0;
            if (fadeWaveCount < fadeWaveSize) {
                if (fadeWaveScaleIndex < fadeWaveScale) {
                    fadeWaveScaleIndex++;    
                } else {
                    fadeWaveScaleIndex = 0;
                    fadeWaveCount++;
                }
            }/* else {
                if (staccatoMode) {
                    fadeCount = 0;
                    fadeWaveCount = 0;
                    fadeWaveScaleIndex = 0;
                    fadeInOut = 1;
                }
            }*/
        }    
    } else { // fade out
        if (fadeCount > fadeOutCountMax) {
            fadeCount = 0;
            if (fadeWaveCount < fadeWaveSize) {
                if (fadeWaveScaleIndex < fadeWaveScale) {
                    fadeWaveScaleIndex++;    
                } else {
                    fadeWaveScaleIndex = 0;
                    fadeWaveCount++;
                }
            } else {
                if (!pitchSetZero) {
                    pitchSetZero = true;
                    pitch = 0;   
                }
            }
        }    
    }
    
    
    if (vibratoMode) {
        vibratoUpdateCount++;
        if (vibratoUpdateCount > vibratoUpdateMax) {
            vibratoUpdateCount = 0;
            vibratoIndex++;
            if (vibratoIndex >= vibratoSize) {
                vibratoIndex = 0;   
            }
            ticker.detach();
            ticker.attach_us(this, &OutputHandler::update, vibrato[vibratoIndex]);
        }   
    }
}

void OutputHandler::rotateInstrument() {
    freqSpecGen.rotateInstrument();
}
