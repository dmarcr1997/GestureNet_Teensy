#include "data.h"

/**
* Load simulated training data for testing before sensor implementation
*/
void loadTrainingData() {
  for (int i = 0; i < MAX_SAMPLE_COUNT; i++) {
    float normMove, normRot;
    if (i < 400) { // REST
      normMove = randomf(0.0, 0.05) / 2.5;
      normRot = randomf(0.0, 6.0) / 360.0;
      trainLabels[i] = 0;
    } else if (i < 800) { // MOVEMENT
      normMove = randomf(0.1, 2.5) / 2.5;
      normRot = randomf(0.0, 30) / 360.0;
      trainLabels[i] = 1;
    } else if (i < 1200) { // ROTATION
      normMove = randomf(0.0, 0.1) / 2.5;
      normRot = randomf(125.0, 360.0) / 360.0;
      trainLabels[i] = 2;
    } else { // MOVEMENT + ROTATION
      normMove = randomf(0.25, 2.5) / 2.5;
      normRot = randomf(50, 360.0) / 360.0;
      trainLabels[i] = 3;
    }
    setInputVector(i, normMove, normRot);
  }
  shuffleData();
}

void setInputVector(int i, float a, float b) {
  trainInputs[i][0] = a;
  trainInputs[i][1] = b;
}

void shuffleData() {
  for(int i =0; i < MAX_SAMPLE_COUNT; i++) {
    int j = random(MAX_SAMPLE_COUNT);

    for (int k = 0; k < 2; k++) {
      float temp = trainInputs[i][k];
      trainInputs[i][k] = trainInputs[j][k];
      trainInputs[j][k] = temp;
    }

    int tempLabel = trainLabels[i];
    trainLabels[i] = trainLabels[j];
    trainLabels[j] = tempLabel;
  }
}
