#ifndef IMU_DATA_H
#define IMU_DATA_H
#include "Wire.h"
#include "MPU6050.h"
#include "utils.h"
#include "const_values.h"

extern float trainInputs[][INPUT_COUNT];
extern int trainLabels[];

static MPU6050 imu;

void initIMU();
void readIMUSample(float *buffer);
void collectLabelSamples(int label, int count);
void collectIMUTrainingData();

#endif // IMU_DATA_H
