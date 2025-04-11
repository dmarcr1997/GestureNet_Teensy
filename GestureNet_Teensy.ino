#include "neural_net.h"
#include "data.h"
static const int EPOCHS = 300;
static float averageLoss;
static bool modelTrained = false;

float trainInputs[SAMPLE_COUNT][INPUT_COUNT];
int trainLabels[SAMPLE_COUNT];
float w_hidden[HIDDEN_COUNT][INPUT_COUNT]; 
float b_hidden[HIDDEN_COUNT];
float w_output[OUTPUT_COUNT][HIDDEN_COUNT];
float b_output[OUTPUT_COUNT];
float hidden[HIDDEN_COUNT];
float output[OUTPUT_COUNT];


void setup() {
  Serial.begin(115200); // Start Serial monitor
  delay(1000); // Wait for serial monitor to start

  randomSeed(analogRead(A0));

  //load in synthetic data from data.h
  loadTrainingData();

  //Initialize weights and biases of model
  initWeights();

  // Train network
  Serial.println("Training started...");
  averageLoss = trainAll(trainInputs, trainLabels, EPOCHS, SAMPLE_COUNT);

  Serial.print("Training complete. Final loss:");
  Serial.println(averageLoss);

  if (averageLoss < 0.05) {
    Serial.println("Model Ready For Prediction...");
    modelTrained = true;
  } else {
    Serial.println("Model did not converge. Try again");
  }
}

void loop() {
  if(!modelTrained) return;

  if(Serial.available()) {
    //Get values to run from Serial, remove commas, and save to inputVector
    String inputValues = Serial.readStringUntil('\n');
    float inputVect[6];
    parseSerialInput(inputValues, inputVect);

    //Run prediction
    float output[4];
    forwardPass(inputVect, output);

    int predicted = predictClass(output);
    String predictedClass;
    switch (predicted) {
      case 0:
        predictedClass = "Rest";
        break;
      case 1:
        predictedClass = "Up/Down";
        break;
      case 2:
        predictedClass = "Left/Right";
        break;
      case 3:
        predictedClass = "Turn";
        break;
      default:
        predictedClass = "Undefined";
        break;
    }
    Serial.print("Predicted Class: ");
    Serial.println(predictedClass);
  }
}

/**
* Used to get 6 input numbers from string sent over USART
* - removes commas
* - validates input
*/
void parseSerialInput(String inputLine, float inputVector[6]) {
  int startIndex = 0;
  int commaIndex;
  for(int i = 0; i < 6; i++) {
    commaIndex = inputLine.indexOf(',', startIndex);
    if (commaIndex == -1 && i < 5) {
      // Not enough commas — input was invalid
      inputVector[i] = 0.0;
      continue;
    }

    String valueString = (commaIndex == -1)
      ? inputLine.substring(startIndex)
      : inputLine.substring(startIndex, commaIndex);

    inputVector[i] = valueString.toFloat();

    startIndex = commaIndex + 1;
  }
}