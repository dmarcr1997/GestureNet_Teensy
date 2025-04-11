#include "data.h"
/**
* Load simulated training data for testing before sensor implementation
*/
void loadTrainingData() {
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    if (i < 400) { //REST
      setInputVector(i, randomf(-0.02, 0.02), randomf(-0.02, 0.02), randomf(-0.02, 0.02), randomf(-0.02, 0.02), randomf(-0.02, 0.02), randomf(-0.02, 0.02));
      trainLabels[i] = 0;
    } else if (i < 800) { //UP/DOWN
      setInputVector(i, 0.0, 0.0, randomf(-1.0, 1.0), 0.0, randomf(-1.0, 1.0), 0.0);
      trainLabels[i] = 1;
    } else if (i < 1200) { //LEFT/RIGHT
      setInputVector(i, randomf(-1.0, 1.0), 0.0, 0.0, randomf(-1.0, 1.0), 0.0, 0.0);
      trainLabels[i] = 2;
    } else { //Turn
      setInputVector(i, 0.0, 0.0, 0.0, 0.0, 0.0, randomf(-1.0, 1.0));
      trainLabels[i] = 3;
    }
  }
  shuffleData();
}

void setInputVector(int i, float a, float b, float c, float d, float e, float f) {
  trainInputs[i][0] = a;
  trainInputs[i][1] = b;
  trainInputs[i][2] = c;
  trainInputs[i][3] = d;
  trainInputs[i][4] = e;
  trainInputs[i][5] = f;
}

void shuffleData() {
  for(int i =0; i < SAMPLE_COUNT; i++) {
    int j = random(SAMPLE_COUNT);

    for (int k = 0; k < 6; k++) {
      float temp = trainInputs[i][k];
      trainInputs[i][k] = trainInputs[j][k];
      trainInputs[j][k] = temp;
    }

    int tempLabel = trainLabels[i];
    trainLabels[i] = trainLabels[j];
    trainLabels[j] = tempLabel;
  }
}
