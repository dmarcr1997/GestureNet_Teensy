#ifndef NEURALNET_H
#define NEURALNET_H
#include "utils.h"
#include "data.h"
//CONSTS
static const int HIDDEN_COUNT = 6; //Hidden layer has 4 neurons
static const int INPUT_COUNT = 6; //6 Inputs from IMU
static const int OUTPUT_COUNT = 4; //Output layer has 4 neurons 
static const float learningRate = 0.01;

// CACHING VARS
extern float w_hidden[HIDDEN_COUNT][INPUT_COUNT]; // weight for each input to hidden layer
extern float b_hidden[HIDDEN_COUNT]; //bias per neuron
extern float w_output[OUTPUT_COUNT][HIDDEN_COUNT]; // weights for output between hidden layer and output layer
extern float b_output[OUTPUT_COUNT]; //bias per neuron
extern float hidden[HIDDEN_COUNT]; // stores current hidden layer activation
extern float output[OUTPUT_COUNT]; // store current output of network


//FUNCTION PROTOS
void initWeights();
void forwardPass(float input[], float output[]);
float computeLoss(float output[], int label);
void trainSample(float input[], int label);
float trainAll(float input[][INPUT_COUNT], int label[], int epochs, int samples);
int predictClass(float output[]);
float relu(float x);
float leakyRelu(float x);
float reluDerivative(float x);
float leakyReluDerivative(float x);

void softmax(float input[], float output[], int len);

#endif //NEURALNET_H