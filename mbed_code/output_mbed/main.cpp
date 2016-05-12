/*
 * OUTPUT
 */

#include "mbed.h"
#include "inputHandler.h"
#include "outputHandler.h"
#include "frequencySpectrumGenerator.h"
#include "wave.h"
#include "math.h"
#include <string>

uint32_t WIDTH = 1;
uint32_t SAMPLE_RATE = 22000;

Serial pc(USBTX, USBRX);
InputHandler inputHandler(p20, 80);
OutputHandler outputHandler("violin", SAMPLE_RATE);
Ticker ticker;

float currentPitch;
float octave;
bool whistle = false;

float keyToPitch(char k) {
    float pitch = 0;
    switch (k) {
        case 'a':
            pitch = 220;
            break;
        case 'w':
            pitch = 233;
            break;
        case 's':
            pitch = 247;
            break;
        case 'd':
            pitch = 262;
            break;
        case 'r':
            pitch = 278;
            break;
        case 'f':
            pitch = 294;
            break;
        case 't':
            pitch = 311;
            break;
        case 'g':
            pitch = 330;
            break;
        case 'h':
            pitch = 349;
            break;
        case 'u':
            pitch = 370;
            break;
        case 'j':
            pitch = 392;
            break;
        case 'i':
            pitch = 415;
            break;
        case 'k':
            pitch = 440;
            break;
        case 'o':
            pitch = 466;
            break;
        case 'l':
            pitch = 494;
            break;
        case 'z':
            pitch = 523;
            break;
        case 'x':
            pitch = 554;
            break;
        case 'c':
            pitch = 587;
            break;
        case 'v':
            pitch = 622;
            break;
        case 'b':
            pitch = 659;
            break;
        case 'n':
            pitch = 699;
            break;
        case '1':
            //octave up
            if (octave < 2) {
                octave++;   
            }
            //pitch = currentPitch;
            break;
        case '2':
            //octave down
            if (octave > -2) {
                octave--;    
            }
            //pitch = currentPitch;
            break;
        case '9':
            //slight up
            pitch = currentPitch + 3;
            break;
        case '0':
            pitch = currentPitch - 3;
            break;
        case '8':
            outputHandler.rotateInstrument();
            break;
        case '.':
            outputHandler.setStaccatoMode();
            break;
        case ',':
            outputHandler.setVibratoMode();
            break;
        default:
            pitch = 0;
            break;
    }
    currentPitch = pitch;
    return pitch * pow((float)2.0, (float)(octave));
}

void updatePitch() {
    float currentPitch = inputHandler.getPitch();
    //pc.printf("%f\n\r", currentPitch);
    outputHandler.setPitch(currentPitch);
}

int main() {
    
    outputHandler.beginOutput();
        
    currentPitch = 440;
    octave = 0;
        
    while(1) {
        /*
        float inputPitch;
        pc.scanf("%f", &inputPitch);
        pc.printf("%f\n\r", inputPitch);
        outputHandler.setPitch(inputPitch);
        */
        if (!whistle) {
            char k = pc.getc();
            if (k == '-') {
                whistle = true;
                inputHandler.startSampling();
                ticker.attach(&updatePitch, 1.0f/ 30);
                outputHandler.setPitch(0);
                outputHandler.setToWhistleMode();
            } else {
                float pitch = keyToPitch(k);
                //pc.printf("%f\n\r", pitch);
                if (pitch == 0) {
                    outputHandler.setPitch(0);   
                } else {
                    outputHandler.setPitch(pitch);                     
                }
            }
        } else {
            char k = pc.getc();
            float pitch = keyToPitch(k);
        }
        
    }
}
