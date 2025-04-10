void initWeights();
void forwardPass(float input[], float output[]);
float computeLoss(float output[], int label);
void trainSample(float input[], int label);
void trainAll();
int predictClass(float output[]);
float relu(float x);
float reluDerivative(float x);
// CACHING VARS
extern float w_hidden[HIDDEN][INPUTS]; // weight for each input to hidden layer
extern float b_hidden[HIDDEN]; //bias per neuron
extern float w_output[OUTPUT][HIDDEN]; // weights for output between hidden layer and output layer
extern float b_output[OUTPUT]; //bias per neuron
extern float hidden[HIDDEN]; // stores current hidden layer activation
extern float output[OUTPUT]; // store current output of network

//CONSTS
extern int HIDDEN;
extern int INPUTS:
extern int OUTPUT; 