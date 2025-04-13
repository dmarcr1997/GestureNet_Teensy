#include "imu_data.h"

/**
* Initialize IMU using library
*/
void initIMU() {
  Wire.begin();
  imu.initialize();
  if(!imu.testConnection()) {
    Serial.println("Failed to connect to IMU...");
    while(1); // Stop program execution
  }
  Serial.println("IMU READY");
}

/**
* Read data from IMU and normalize data
* - modify buffer in place since it is passed in as a pointer
*/
void readIMUSample(float *buffer) {
  int16_t ax, ay, az, gx, gy, gz;
  imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float axf = (float)ax / 16384.0;
  float ayf = (float)ay / 16384.0;
  float azf = (float)az / 16384.0;
  float gxf = (float)gx / 131.0;
  float gyf = (float)gy / 131.0;
  float gzf = (float)gz / 131.0;

  float move = sqrt(axf * axf + ayf * ayf + azf * azf) - 1.0; // subtract gravity
  float rotate = sqrt(gxf * gxf + gyf * gyf + gzf * gzf);

  buffer[0] = constrain(fabs(move) / 2.5, 0.0, 1.0);
  buffer[1] = constrain(rotate / 360.0, 0.0, 1.0);
}

/**
* Collect <= 400 samples for each gesture type
*/
void collectLabelSamples(int label, int count, String labelName) {
  float input[INPUT_COUNT];
  Serial.print("Collecting ");
  Serial.print(labelName);
  Serial.println(" in 2 seconds.");
  delay(2000);
  Serial.println("START...");
  delay(100);
  int startingIndex = label * count;
  int endingIndex = startingIndex + count;

  for(int i = startingIndex; i < endingIndex; i++) {
    readIMUSample(input);
    for(int j = 0; j < INPUT_COUNT; j++) {
      trainInputs[i][j] = input[j];
    } // Move data from buffer to trainInput data
    trainLabels[i] = label; // save label
    delay(10); // delay 100 ms
  }
}

/**
* Initializes data collection to get 400 data samples for each label
*/
void collectIMUTrainingData() {
  collectLabelSamples(0, 400, "STATIONARY"); // REST
  collectLabelSamples(1, 400, "MOVEMENT"); // UP/DOWN
  collectLabelSamples(2, 400, "ROTATION"); // LEFT/RIGHT
  collectLabelSamples(3, 400, "MOVEMENT + ROTATION"); // TURN
}