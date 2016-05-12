#include "frequencySpectrumGenerator.h"


Predictor::Predictor() {
    numberOfTerms = 0;
    coefficients_f = NULL; 
}

void Predictor::addCoefficient(float coefficient) {
    coefficients.push_back(coefficient);
    numberOfTerms++;
}

/*
//call when done adding coefficients!!!
void Predictor::convertToFloatArray() {
    if (coefficients_f == NULL) {
        coefficients_f = new float[numberOfTerms];
        int index = 0;
        for (std::vector<float>::iterator coeff = coefficients.begin() ; coeff != coefficients.end(); ++coeff) { 
            coefficients_f[index] = (float) *coeff;
        }   
    }
}
*/

float Predictor::predict(float pitch) {
    
    float result = 0;
    int currentOrder = numberOfTerms - 1;
    
    for (std::vector<float>::iterator coeff = coefficients.begin() ; coeff != coefficients.end(); ++coeff) { 
        
        result += (float) *coeff * pow(pitch, currentOrder);
        currentOrder -= 1;
    }
    
    return result;
    
    /*
    float result = 0;
    int currentOrder = numberOfTerms - 1;
    
    for (int i = 0; i < numberOfTerms; i++) {
        result += coefficients_f[i] * pow(pitch, currentOrder);
        currentOrder--;   
    }
    return result;
    */
}

FrequencySpectrumGenerator::FrequencySpectrumGenerator(string filename) : pc(USBTX, USBRX), _interrupt(p12){
    numberOfHarmonics = 0;
    instrumentIndex = 0;
    instruments[0] = "flute";
    instruments[1] = "violin";
    instruments[2] = "tuba";
    instruments[3] = "clarinet";
    instruments[4] = "trumpet";
    
    FrequencySpectrumGenerator::readFile(instruments[instrumentIndex]);
    _interrupt.fall(this, &FrequencySpectrumGenerator::rotateInstrument);
}



void FrequencySpectrumGenerator::readFile(string instrument) {
    /*
    LocalFileSystem local("local");
    char buffer [2000];
    string line;
    
    
    FILE *fp = fopen("/local/test.txt", "r");
    if (fp != NULL) {
        if (fgets(buffer, 1000, fp) != NULL) {
            std::istringstream f(buffer);
            while (std::getline(f, line, ',')) {
                Predictor predictor;
                pc.printf("%s\r\n", line.c_str());
                stringstream ss(line);
                string buf;
                while (ss >> buf) {
                    double val = std::atof (buf.c_str());
                    float val_f = (float) val;
                    predictor.addCoefficient(val_f);
                }
                harmonicPredictors.push_back(predictor);
            }
        }
    } else {
        pc.printf("null file\n\r");   
    }
    fclose(fp);
    */
    //string flute = "0.0833,0.0412,0.0509,0.0185,0.0048,0.0097,0.0009,0.0009,0.0003,0.0002,";
    //string tuba = "0.0000538081 -0.0108085874 0.6739241728,-0.0002336671 0.0745524952 -5.1354372964,0.0001417889 -0.0538671649 5.0870593178,-0.0000002187 -0.0005704312 0.1466741683,-0.0000059494 0.0017460078 -0.0982898545,";
    //string violin = "-0.0000006993 0.0012459093 0.2894867194,-0.0000005163 -0.0002330333 0.6715325893,-0.0000020625 0.0025384442 -0.5133234420,-0.0000034264 0.0041923883 -0.9956716282,-0.0000000213 -0.0001546834 0.2037063082,";
    //string clarinet = "0.5,";
    
    string trumpet = "0.0000001070 0.0017774125 -0.3423704026,0.0000166068 -0.0117279672 2.2763826477,-0.0000248857 0.0161332611 -2.2739260116,0.0000013497 -0.0012628073 0.4443051832,-0.0000145923 0.0089745063 -1.0725343416,";
    string violin = "-0.0000004979 0.0008870280 0.2061007360,-0.0000003676 -0.0001659086 0.4780991721,-0.0000014684 0.0018072512 -0.3654618057,-0.0000024394 0.0029847805 -0.7088707068,-0.0000000152 -0.0001101272 0.1450291749,";
    string flute = "-0.0000000841 -0.0008772525 0.9022668464,-0.0000000601 -0.0001433220 0.1607064625,0.0000000362 -0.0000870850 0.0486296936,-0.0000000497 0.0000611952 -0.0159247992,-0.0000000579 0.0000716406 -0.0190012987,";
    string clarinet = "0.5,0.01,0.3,0.01,0.1";
    string tuba = "0.0000000000 -0.0000000000 0.5000000000,0.0000035257 -0.0041903934 1.3672098486,0.0000024803 -0.0037375720 1.4160557599,0.0000002425 -0.0003946413 0.1714192206,0.0000000395 -0.0000879930 0.0491200397,";
    
    /*
    string trumpet = "0.0000001070 0.0017774125 -0.3423704026,0.0000166068 -0.0117279672 2.2763826477,";
    string violin = "-0.0000004979 0.0008870280 0.2061007360,-0.0000003676 -0.0001659086 0.4780991721,";
    string flute = "-0.0000000841 -0.0008772525 0.9022668464,-0.0000000601 -0.0001433220 0.1607064625,";
    string clarinet = "0.5,0.01,";
    string tuba = "0.0000000000 -0.0000000000 0.5000000000,0.0000035257 -0.0041903934 1.3672098486,";
    */
    
    string vals;
    if (instrument.compare("flute") == 0) {
        vals = flute;
    } else if (instrument.compare("tuba") == 0) {
        vals = tuba;   
    } else if (instrument.compare("violin") == 0) {
        vals = violin;   
    } else if (instrument.compare("clarinet") == 0) {
        vals = clarinet;   
    } else if (instrument.compare("trumpet") == 0) {
        vals = trumpet;
    }
    
    std::istringstream f(vals.c_str());
    string line;
    while (std::getline(f, line, ',')) {
        numberOfHarmonics++;
        Predictor predictor;
        stringstream ss(line);
        string buf;
        while (ss >> buf) {
            float val = std::atof (buf.c_str());
            predictor.addCoefficient(val);
        }
        harmonicPredictors.push_back(predictor);
    }
    pc.printf("success!\n\r");
    
}

std::vector<float>& FrequencySpectrumGenerator::getHarmonicAmplitudes(float pitch) {
    //std::vector<float> harmonicAmplitudes;
    harmonicAmplitudes.clear();
    for (std::vector<Predictor>::iterator predictor = harmonicPredictors.begin(); predictor != harmonicPredictors.end(); ++predictor) {
        Predictor p = (Predictor) *predictor;
        float term = p.predict(pitch);
        harmonicAmplitudes.push_back(term);
        //harmonicAmplitudes[index] = term;
        //index++;
    } 
    
    return harmonicAmplitudes;
}

int FrequencySpectrumGenerator::getNumberOfHarmonics() {
    return numberOfHarmonics;
}

void FrequencySpectrumGenerator::rotateInstrument() {
    instrumentIndex++;
    if (instrumentIndex >= 5) {
        instrumentIndex = 0;   
    }
    harmonicPredictors.clear();
    pc.printf("%s\n\r", instruments[instrumentIndex].c_str());
    FrequencySpectrumGenerator::readFile(instruments[instrumentIndex]);
}