
# Edge-Native Predictive Maintenance: K-Means on ESP8266

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Hardware: ESP8266](https://img.shields.io/badge/Hardware-ESP8266-blue.svg)](https://www.espressif.com/en/products/socs/esp8266)
[![Framework: Edge Impulse](https://img.shields.io/badge/Framework-Edge_Impulse-orange.svg)](https://edgeimpulse.com/)

This repository contains the firmware, datasets, and DSP scripts for deploying a highly optimized, edge-native Machine Learning (TinyML) pipeline engineered for real-time vibration anomaly detection on strictly constrained microcontrollers.

## 📝 Abstract & Overview
Predictive maintenance is critical in industrial IoT, but deploying intelligent diagnostics on legacy hardware (e.g., sub-100 KB RAM) is challenging. This project demonstrates a complete offline condition-monitoring pipeline.

By utilizing a Fast Fourier Transform (FFT) and Root Mean Square (RMS) calculations, chaotic 50 Hz time-domain accelerometer data is compressed into lightweight spectral features. An unsupervised K-Means clustering model then evaluates these features to classify the physical state of a DC motor with 100% test accuracy, using only **4.2 KB of RAM**.

## ⚙️ Hardware Architecture
* **Microcontroller:** ESP8266 (NodeMCU V3) - *80 MHz CPU, 80 KB SRAM*
* **Sensor:** MPU6050 6-Axis IMU (Interfaced via I2C at 100 kHz)
* **Actuator:** 3V-6V DC Motor (Physical Testbed)
* **Alerts:** Standard GPIO LED


## 🚀 How to Run the Inference

  1. Hardware Setup: Connect the MPU6050 to the ESP8266 (SDA to D2, SCL to D1). Connect an LED to pin D7 for hardware alerts.

  2. Library Installation: Install the Adafruit MPU6050 library and your exported Edge Impulse Inferencing library via the Arduino IDE Library Manager.

  3. Flash Firmware: Open the .ino file located in the /firmware folder and upload it to the NodeMCU V3.

  4. Monitor: Open the Serial Monitor at 115200 baud. The system will continuously sample 1-second windows at 50 Hz and output the calculated mathematical anomaly score.

# Operational State Thresholds

The firmware dynamically categorizes the motor into three states based on empirical Euclidean distance from the K-Means centroid:

  * Nominal Operation: Score < 3.5

  * Offline State: 3.5 < Score < 10.0

  * Critical Imbalance: Score > 10.0 (Triggers GPIO HIGH)


## 📊 Digital Signal Processing (DSP)

Running inference on raw time-series data instantly overflows the ESP8266 memory. Included in the /docs folder is a Python script (plot_fft.py) that visually demonstrates the DSP pipeline. It extracts the dominant 13 Hz rotational frequency peak, which serves as the primary feature for the K-Means algorithm.


## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.
