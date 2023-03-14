#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int stepThreshold = 1000; // adjust this threshold to your liking

int lastYAccel = 0;
bool isStep = false;
unsigned long stepStartTime = 0;
unsigned int stepCount = 0;
float distance = 0.0;
float cadence = 0.0;
float gaitSpeed = 0.0;
float stepLength = 0.0;
float stepTime = 0.0;
float swingTime = 0.0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
}

void loop() {
  // read accelerometer data
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // calculate cadence
  if (abs(ay) > stepThreshold) {
    if (!isStep) {
      isStep = true;
      stepCount++;
      unsigned long currentTime = millis();
      if (stepCount == 1) {
        stepStartTime = currentTime;
      } else {
        cadence = 60000.0 / (currentTime - stepStartTime) * stepCount;
      }
    }
  } else {
    isStep = false;
  }

  // calculate gait speed, step length, step time, and swing time
  if (stepCount > 0) {
    gaitSpeed = distance / ((millis() - stepStartTime) / 1000.0);
    stepLength = gaitSpeed * ((millis() - stepStartTime) / 1000.0) / stepCount;
    stepTime = ((millis() - stepStartTime) / 1000.0) / stepCount;
    swingTime = stepTime / 2.0;
  }

  // print the results
  Serial.print("Cadence: ");
  Serial.print(cadence);
  Serial.print(" steps per minute, Gait speed: ");
  Serial.print(gaitSpeed);
  Serial.print(" m/s, Step length: ");
  Serial.print(stepLength);
  Serial.print(" m, Step time: ");
  Serial.print(stepTime);
  Serial.print(" s, Swing time: ");
  Serial.print(swingTime);
  Serial.println(" s");

  // update distance
  if (isStep && lastYAccel < ay) {
    distance += stepLength;
  }
  lastYAccel = ay;

  delay(100); // adjust the delay to your liking
}
