#include "data.h"
// SET CONST
SAMPLE_COUNT = 400;

/**
* Load simulated training data for testing before sensor implementation
*/
void loadTrainingData() {
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    if (i < 100) {
      trainInputs[i] = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0];
      trainLabels[i] = 0;
    } else if (i < 200) {
      trainInputs[i] = [0.0, 0.0, randomf(0.6, 1.0), 0.0, randomf(0.5, 1.0), 0.0];
      trainLabels[i] = 1;
    } else if (i < 300) {
      trainInputs[i] = [randomf(0.6, 1.0), 0.0, 0.0, randomf(0.6, 1.0), 0.0, 0.0];
      trainLabels[i] = 2;
    } else {
      trainInputs[i] = [0.0, 0.0, 0.0, 0.0, 0.0, randomf(0.6, 1.0)];
      trainLabels[i] = 3;
    }
  }
}