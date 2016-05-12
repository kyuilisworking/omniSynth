#ifndef FILTERS_H
#define FILTERS_H

#include <math.h>
#include "wave.h"

class Filters {
    public:
        static void lowPass(Wave& wave, int samplingRate, int cutoffFreq);
};

#endif