#ifndef WAVE_H
#define WAVE_H

class Wave {
public:
    Wave(float* data, int dataSize);
    ~Wave();
    float* getData(void);
    int getDataSize(void);
    void setData(float* data, int dataSize);

private:
    float* data;
    int dataSize;
};

#endif