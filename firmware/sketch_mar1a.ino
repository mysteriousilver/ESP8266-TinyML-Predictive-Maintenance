#include <anomaly_detection_inferencing.h>

/* 2. INCLUDE HARDWARE LIBRARIES */
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define ALERT_LED D7
Adafruit_MPU6050 mpu;

/* Allocate a buffer to hold 1 second of vibration data */
float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Edge Impulse TinyML Anomaly Detector initialized!");

  // Start I2C on NodeMCU pins D2(SDA) and D1(SCL)
  Wire.begin(D2, D1); 
  
  if (!mpu.begin(0x68)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) { delay(10); }
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(ALERT_LED, OUTPUT);
  digitalWrite(ALERT_LED, LOW); // Ensure it starts turned off
}

void loop() {
  Serial.println("\nRecording 1 second of data...");

  // Fill the buffer with 50 readings (50Hz = 20ms delay)
  for (int i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i += 3) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    features[i + 0] = a.acceleration.x;
    features[i + 1] = a.acceleration.y;
    features[i + 2] = a.acceleration.z;

    delay(20); 
  }

  // Convert the array into a signal the AI can understand
  signal_t signal;
  int err = numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
  if (err != 0) {
      Serial.printf("Failed to create signal (%d)\n", err);
      return;
  }

  // Run the AI Classifier
  ei_impulse_result_t result = { 0 };
  err = run_classifier(&signal, &result, false);
  if (err != EI_IMPULSE_OK) {
      Serial.printf("Failed to run classifier (%d)\n", err);
      return;
  }

  // Print the final Anomaly Score!
  Serial.printf("Anomaly Score: %.3f\n", result.anomaly);

  // Trigger an alert if the score breaks our Safe Zone threshold
  if (result.anomaly > 10.0) { 
      Serial.println("⚠️ WARNING: CRITICAL IMBALANCE DETECTED! ⚠️");
      digitalWrite(ALERT_LED, HIGH);
  } 
  else if (result.anomaly > 3.5) {
      Serial.println("💤 MOTOR OFF (Or missing standard vibration)");
      digitalWrite(ALERT_LED, LOW);
  } 
  else {
      Serial.println("✅ Motor running smoothly.");
      digitalWrite(ALERT_LED, LOW);
  }
  
  delay(1000); // Wait a second before checking again
}