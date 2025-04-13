# Teensy 4.1 Neural Network: Real-Time Gesture(Psuedo) Classification

Neural network I implemented from scratch on a Teensy 4.1 microcontroller. It processes IMU data in real-time to classify motion types such as Rest, Movement, Rotation, and Combined Movement + Rotation(Not too well on this last one yet). This project serves as a proof of concept for efficient on-device inference and will evolve into FPGA-based neural control systems to emulate memristors. Eventually I want to take this and push for neural network based control systems that can change weights, biases, and eventually the circuit as a whole to change on the fly based on environmental feedback.

---

## 🤖 Features

- Fully custom neural network engine (no libraries).
- Softmax output layer + cross-entropy loss.
- Leaky ReLU hidden activation.
- In-memory training and prediction on Teensy 4.1(Using synthetic data for now. The IMU I had was not fast enough for on the fly training/data colelction).
- IMU input via MPU6050 (6-axis accelerometer + gyroscope).
- Normalized data ranges (0–1) for stable training(Tried with out this and ran into a lot of major issues).

---

## 🧩 Architecture

```text
[ IMU Sensor ]
     ↓
[ Normalize ]
     ↓
[ Neural Net ]
  - 2 Inputs
  - 4 Hidden Neurons
  - 4 Outputs
     ↓
[ Prediction ]
     ↓
[ Serial Output ]
```

## 🧪 Gesture Classes

| Label | Gesture Type         | Description                       |
|-------|----------------------|-----------------------------------|
| 0     | **Stationary**       | No motion, resting state          |
| 1     | **Movement**         | Translational motion only         |
| 2     | **Rotation**         | Rotational movement only          |
| 3     | **Movement+Rotation**| Both movement and rotation        |

---

## 📦 Files & Structure

| File                          | Description                                                                    |
|-------------------------------|-------------------------------------------------------------------------------|
| `neural_net.cpp`              | Core neural network engine                                                    |
| `imu_data.cpp`                | IMU data reading and preprocessing (also used to collect some data)           |
| `data.cpp`                    | Training data generator (synthetic)                                           |
| `GestureNet_Teensy.ino`       | Entry point, runs setup + prediction loop                                     |
| `const_values.h`              | Constants (hidden size, learning rate, etc.)                                  |

---

## 🔧 Configuration

Update these in `const_values.h`:

```cpp
static const int INPUT_COUNT = 2;
static const int HIDDEN_COUNT = 4;
static const int OUTPUT_COUNT = 4;
static const float learningRate = 0.01;
static const int MAX_SAMPLE_COUNT = 1600;
static const int EPOCHS = 300;
```

## Example Output
```bash
Sample 0.01, 3.52 | Label: ROTATION
Output: 0.01 0.03 0.95 0.01 → Predicted: ROTATION

Sample 2.25, 5.83 | Label: MOVEMENT + ROTATION
Output: 0.02 0.04 0.11 0.83 → Predicted: MOVEMENT + ROTATION
```

## 🛠️ Dependencies
* MPU6050
* Teensyduino + Arduino IDE
* Wire.h
* Standard C++ (Teensy 4.1)

## 🚀 Getting Started
* Connect MPU6050 to Teensy via I2C(SDC/A4, SCL/A5).
* Upload program using Arduino IDE.
* Open serial monitor(115200 baud).

* Wait for model to train.
* If it fails change some params or the training data and try again.
* Type "START" to run a gesture classification
  * Note: Rest, Movement, and Rotation all work but Movement + Rotation is not consistently working yet.


## Future Plans
I want to rebuild a more advanced version of this on a FPGA (Tang Nano 20K) to emulate memristors as the neurons.
Then from here I plan on trying to integrate PID/Kalman-style control logic and using it to run something, I am not sure what yet.

## 👨‍💻 Author
Built by Damon Rocha — inspired by robotics, nature, and AI to solve real-world problems.

📄 License
[MIT License](./LICENSE)