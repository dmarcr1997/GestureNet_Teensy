#include "neural_net.h"
#include "imu_data.h"
#include "data.h"
#include "const_values.h"

static float averageLoss;
static bool modelTrained = false;

float trainInputs[MAX_SAMPLE_COUNT][INPUT_COUNT];
int trainLabels[MAX_SAMPLE_COUNT];
float w_hidden[HIDDEN_COUNT][INPUT_COUNT]; 
float b_hidden[HIDDEN_COUNT];
float w_output[OUTPUT_COUNT][HIDDEN_COUNT];
float b_output[OUTPUT_COUNT];
float hidden[HIDDEN_COUNT];
float output[OUTPUT_COUNT];

void debugIMU() {
  float inputs[INPUT_COUNT];
  readIMUSample(inputs);
  Serial.println("DEBUG");
  Serial.println("MOVEMENT\tROTATION");
  for(auto i : inputs) {
    Serial.print(i);
    Serial.print("\t");
  }
  Serial.println("DONE");
}

void setup() {
  Serial.begin(115200); // Start Serial monitor
  delay(1000); // Wait for serial monitor to start

  randomSeed(analogRead(A0));
  initIMU();
  loadTrainingData();

  //Initialize weights and biases of model
  initWeights();

  // Train network
  Serial.println("Training started...");
  averageLoss = trainAll(trainInputs, trainLabels, EPOCHS, MAX_SAMPLE_COUNT);

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
  // debugIMU();
  if(!modelTrained) return;

  if(Serial.available()) {
    //Get values to run from Serial, remove commas, and save to inputVector
    String inputValues = Serial.readStringUntil('\n');
    // float inputVect[6];
    // parseSerialInput(inputValues, inputVect);
    if(inputValues == "START") {
      Serial.println("RUNNING");
    }
    float inputVect[2];
    Serial.println("Begin gesture in 3 seconds...");
    delay(3000);
    Serial.println("START");
    delay(1000);
    readIMUSample(inputVect);
    Serial.println("DEBUG");
    Serial.println("MOVEMENT\tROTATION");
    for(auto i : inputVect) {
      Serial.print(i);
      Serial.print("\t");
    }
    Serial.println("DONE");

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
        predictedClass = "Movement";
        break;
      case 2:
        predictedClass = "Rotation";
        break;
      case 3:
        predictedClass = "Movement + Rotation";
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