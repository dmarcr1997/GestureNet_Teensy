#include "data.h"
/**
* Load simulated training data for testing before sensor implementation
*/
void loadTrainingData() {
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    if (i < 200) { //REST
      setInputVector(i, randomf(-0.1, 0.1), randomf(-0.1, 0.1), randomf(-0.1, 0.1), randomf(-0.1, 0.1), randomf(-0.1, 0.1), randomf(-0.1, 0.1));
      trainLabels[i] = 0;
    } else if (i < 400) { //UP/DOWN
      setInputVector(i, 0.0, 0.0, randomf(-1.0, 1.0), 0.0, randomf(-1.0, 1.0), 0.0);
      trainLabels[i] = 1;
    } else if (i < 600) { //LEFT/RIGHT
      setInputVector(i, randomf(-1.0, 1.0), 0.0, 0.0, randomf(-1.0, 1.0), 0.0, 0.0);
      trainLabels[i] = 2;
    } else { //Turn
      setInputVector(i, 0.0, 0.0, 0.0, 0.0, 0.0, randomf(-1.0, 1.0));
      trainLabels[i] = 3;
    }
  }
}

void setInputVector(int i, float a, float b, float c, float d, float e, float f) {
  trainInputs[i][0] = a;
  trainInputs[i][1] = b;
  trainInputs[i][2] = c;
  trainInputs[i][3] = d;
  trainInputs[i][4] = e;
  trainInputs[i][5] = f;
}
