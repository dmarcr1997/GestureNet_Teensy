//INIT CONSTS
HIDDEN = 4; //Hidden layer has 4 neurons
INPUTS = 6; //6 Inputs from IMU
OUTPUT = 4; //Output layer has 4 neurons 

/**
* Function to initialize weights of neurons
*/
void initWeights()
{
  //Init weights and biases of hidden layer to random values
  for (int i = 0; i < HIDDEN; i++) {
    for (int j = 0; j < INPUTS; j++) {
      w_hidden[i][j] = randomf(0.0, 1.0);
    }
    b_hidden[i] = 0.0;
  }

  //Init weights and biases of output layer to random values
  for (int i = 0; i < OUTPUT; i++) {
    for (int j = 0; j < HIDDEN; j++) { //HIDDEN Layer count is the input neurons to OUTPUT layer
      w_output[i][j] = randomf(0.0, 1.0);
    }
    b_output[i] = 0.0;
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
  for (int i = 0; i < HIDDEN; i++) {
    float sum = 0.0;
    for (int j = 0; j < INPUTS; j++) {
      //Weighted sum of inputs for each neuron in hidden layer
      sum += inputs[j] * w_hidden[i][j];
    }
    // Add in bias
    sum += b_hidden[i];
    //Apply ReLu
    hidden[i] = relu(sum);
  }

  // Compute output layer
  for (int k = 0; k < OUTPUT; k++) {
    float sum = 0.0;
    for (int m = 0; m < HIDDEN; m++) {
      // Weighted sum of neurons again (y = x * w1 * w2); where x * w1 = h;
      // For each neuron run y = SUM of all(h * w2) 
      sum += hidden[m] * w_output[i];
    }
    //Add bias to output activation
    sum += b_output[i];
    // Save to output
    output[i] = sum;
  }
}

/**
* MSE One-hot encoded loss
* - for prediction labels {up, down, left, right}
* - we would replace each with 0 that is not the input label
* - then apply MSE to the label based on the output difference
*/
float computeLoss(float output[], int label) {
  float loss = 0.0;
  for (int i =0; i < OUTPUT; i++) {
    float target = (i == label) ? 1.0 : 0.0; // set to 1 for expected value  and 0 for others
    float diff = output[i] - target; // find (y_hat - y)
    loss += 0.5 * diff * diff; // MSE = 1/2(y_hat - y)^2
  }
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

  // create one-hot encoded vector using label
  int oneHot[OUTPUT] = {0};
  oneHot[label] = 1;

  // Backpropagation for outer layer
  for (int i = 0; i < OUTPUT; i++) {
    // compute output error using gradient of loss fn(1/2 (y_hat -y)) which turns inton ()
    error = output[i] - oneHot[i];

    // dL/dq_output (gradient of output weight) = error * hidden[j]
    for (int j = 0; j < HIDDEN; j++) {
      gradOut = error * hidden[j];
      w_output[i][j] -= learningRate * gradOut; //Update output layer weights in directon of -gradient
    }
    // Use error to update bias of output layer
    b_output[i] -= learningRate * error;
  }

  // Backpropagation for hidden layer
  for (int k = 0; k < HIDDEN; k++) {
    float sumGradient = 0.0;

    // dL/dhidden[k] = sum of error * weights
    for(int m = 0; m < OUTPUTS; m++) {
      sumGradient += (output[m] - ontHot[m]) * w_output[m][k];
    }

    // multiply by derivative of relu to factor in activation function
    sumGradient *= reluDerivative(hidden[i]);

    // dL/dw_hidden = sumGradient * input[n]
    for(int n = 0; n < INPUTS; n++) {
      float gradientHidden = sumGradient * input[j];
      w_hidden[k][n] -= learningRate * gradientHidden; //update hidden layer weights in direction of -gradient 
    }

    // use gradient to update bias of hidden layer
    b_hidden[k] -= learningRate * sumGradient;
  }
}

void trainAll();
int predictClass(float output[]);


float relu(float x) {
  return x > 0 ? x : 0;
}

float reluDerivative(float x) {
  return x > 0 ? 1 : 0;
}