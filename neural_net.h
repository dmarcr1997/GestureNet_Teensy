void initWeights();
void forwardPass(float input[], float output[]);
float computeLoss(float output[], float label);
void trainSample(float input[], int label);
void trainAll();
int predictClass(float output[]);