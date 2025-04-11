#ifndef DATA_H
#define DATA_H

#include "utils.h"
void loadTrainingData();
void setInputVector(int i, float a, float b, float c, float d, float e, float f);
void shuffleData();
extern float trainInputs[][6];
extern int trainLabels[];
static const int SAMPLE_COUNT = 1600;

#endif //DATA_H