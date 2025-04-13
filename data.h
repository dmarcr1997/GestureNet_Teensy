#ifndef DATA_H
#define DATA_H

#include "utils.h"
#include "const_values.h"
void loadTrainingData();
void setInputVector(int i, float a, float b);
void shuffleData();
extern float trainInputs[][INPUT_COUNT];
extern int trainLabels[];

#endif //DATA_H