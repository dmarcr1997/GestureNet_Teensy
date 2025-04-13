#include "neural_net.h"
/**
* Function to initialize weights of neurons
*/
void initWeights()
{
  //Init weights and biases of hidden layer to random values
  for (int i = 0; i < HIDDEN_COUNT; i++) {
    for (int j = 0; j < INPUT_COUNT; j++) {
      w_hidden[i][j] = randomf(-0.05, 0.05);
    }
    b_hidden[i] = randomf(-0.01, 0.01);
  }

  //Init weights and biases of output layer to random values
  for (int i = 0; i < OUTPUT_COUNT; i++) {
    for (int j = 0; j < HIDDEN_COUNT; j++) { //HIDDEN Layer count is the input neurons to OUTPUT layer
      w_output[i][j] = randomf(-0.05, 0.05);
    }
    b_output[i] = randomf(-0.01, 0.01);
  }

}

/**
* Forward Pass of neural network. 
* - Multiplies input by its weights and add biases
* - Sums the values
* - Applies ReLu activation for non linearity which is important for 
*   Multiclass classifications like this.
* - Similar approach for the output neuron but leaving out relu.
*/
void forwardPass(float inputs[], float output[]) {
  for (int i = 0; i < HIDDEN_COUNT; i++) {
    float sum = 0.0;
    for (int j = 0; j < INPUT_COUNT; j++) {
      //Weighted sum of inputs for each neuron in hidden layer
      sum += inputs[j] * w_hidden[i][j];
    }
    // Add in bias
    sum += b_hidden[i];
    //Apply ReLu
    hidden[i] = leakyRelu(sum);
  }

  // Compute output layer
  float logits[OUTPUT_COUNT];
  for (int k = 0; k < OUTPUT_COUNT; k++) {
    float sum = 0.0;
    for (int m = 0; m < HIDDEN_COUNT; m++) {
      sum += hidden[m] * w_output[k][m];
    }
    sum += b_output[k];
    logits[k] = sum;
  }

  // Convert logits into probabilities for each label
  softmax(logits, output, OUTPUT_COUNT);
}

/**
* Cross entropy loss
* - Measures how close model's predicted probability(softmax) is to label
* - we would replace each with 0 that is not the input label
* - then apply MSE to the label based on the output difference
*/
float computeLoss(float output[], int label) {
  const float epsilon = 1e-6; // no log of 0
  float loss = -log(output[label] + epsilon); // Cross-entropy: -log(probability of (correct_label))
  return loss;
}

/**
* Helper function to train a single sample
* - Forward pass
* - Calculates loss using one hot MSE
* - Computes Gradient with Chain Rule
* - Updates weights and biases
*/
void trainSample(float input[], int label) {
  // Compute hidden activates and output values
  forwardPass(input, output);
  if (label < 0 || label >= OUTPUT_COUNT) {
    Serial.print("Invalid label: ");
    Serial.println(label);
    return;
  }
  // create one-hot encoded vector using label
  int oneHot[OUTPUT_COUNT] = {0};
  oneHot[label] = 1;

  // Backpropagation for outer layer
  for (int i = 0; i < OUTPUT_COUNT; i++) {
    // compute output error using gradient of loss fn(1/2 (y_hat -y)) which turns inton ()
    float error = output[i] - oneHot[i];

    // dL/dq_output (gradient of output weight) = error * hidden[j]
    for (int j = 0; j < HIDDEN_COUNT; j++) {
      float gradOut = error * hidden[j];
      w_output[i][j] -= learningRate * gradOut; //Update output layer weights in directon of -gradient
    }
    // Use error to update bias of output layer
    b_output[i] -= learningRate * error;
  }

  // Backpropagation for hidden layer
  for (int k = 0; k < HIDDEN_COUNT; k++) {
    float sumGradient = 0.0;

    // dL/dhidden[k] = sum of error * weights
    for(int m = 0; m < OUTPUT_COUNT; m++) {
      sumGradient += (output[m] - oneHot[m]) * w_output[m][k];
    }

    // multiply by derivative of relu to factor in activation function
    sumGradient *= leakyReluDerivative(hidden[k]);

    // dL/dw_hidden = sumGradient * input[n]
    for(int n = 0; n < INPUT_COUNT; n++) {
      float gradientHidden = sumGradient * input[n];
      w_hidden[k][n] -= learningRate * gradientHidden; //update hidden layer weights in direction of -gradient 
    }

    // use gradient to update bias of hidden layer
    b_hidden[k] -= learningRate * sumGradient;
  }
}

/**
* Train model for epoch count 
* - Calls trainSample
* - calculates loss
* - Outputs models average loss
*/
float trainAll(float input[][INPUT_COUNT], int label[], int epochs, int samples) {
  float averageLoss = 0.0;

  Serial.println("Starting training loop...");
  
  for (int i = 0 ; i < epochs; i++) {
    float totalLoss = 0.0;
    // Serial.print("Epoch "); Serial.println(i);

    for (int j = 0; j < samples; j++) {
      trainSample(input[j], label[j]);  // Might crash here
      float loss = computeLoss(output, label[j]);
      totalLoss += loss;
    }

    float avgLoss = totalLoss / samples;
    averageLoss = avgLoss;
    // Serial.print("Avg Loss: ");
    // Serial.println(avgLoss, 5);
  }

  return averageLoss;
}

/**
* Get predicted class based on maximum output 
*/
int predictClass(float output[]) {
  Serial.print("Output: ");
  for (int i = 0; i < OUTPUT_COUNT; i++) {
    Serial.print(output[i], 4);
    Serial.print(" ");
  }
  Serial.println();
  float maxValue = output[0];
  int maxIndex = 0;
  for (int i = 1; i < 4; i++) {
    if (output[i] > maxValue) {
      maxValue = output[i];
      maxIndex = i;
    }
  }
  return maxIndex;
}

/**
* Leaky relu activation function
* - applies non-linearity to hidden-layer outputs
* - if output > 0 return output
* - else return x scaled by 0.01
*/
float leakyRelu(float x) {
  return x > 0 ? x : 0.01 * x;
}


/*
* Used in backpropagation for activation slope
* - similar to above function but only accounting for derivative
* - dRelu/dx = x > 0 return 1 else return 0.01 
*/
float leakyReluDerivative(float x) {
  return x > 0 ? 1.0 : 0.01;
}

/*
* Turns prediction values into a probability distribution between 0-1 all outputs probs add up to be 1
* - Adding this in to help with overfitting/local minimas
*/
void softmax(float input[], float output[], int len) {
  float maxValue = input[0];
  for (int i = 1; i < len; i++) {
    if(input[i] > maxValue) {
      maxValue = input[i];
    }
  }

  float sum = 0.0;
  for (int i = 0; i < len; i++) {
    output[i] = exp(input[i] - maxValue);
    sum += output[i];
    if (!isfinite(output[i])) output[i] = 1e-6;
  }
  for (int i = 0; i < len; i++) {
    output[i] /= sum;
  }
}