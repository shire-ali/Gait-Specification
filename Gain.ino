#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <LiquidCrystal.h>
#include <SD.h>

#define SD_CS 10
#define INTERRUPT_PIN 2

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
File dataFile;

volatile bool interruptTriggered = false;
unsigned long interruptTime;
unsigned long previousInterruptTime;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();

  if (!accel.begin()) {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    while (1);
  }

  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interruptHandler, RISING);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }

  dataFile = SD.open("gait_data.txt", FILE_WRITE);
  dataFile.println("Cadence,Gait Speed,Step Length,Step Time,Stance Time,Swing Time");
  dataFile.close();
}

void loop() {
  if (interruptTriggered) {
    sensors_event_t event;
    accel.getEvent(&event);

    float acceleration = sqrt(event.acceleration.x * event.acceleration.x + event.acceleration.y * event.acceleration.y + event.acceleration.z * event.acceleration.z);
    float cadence = calculateCadence();
    float gaitSpeed = calculateGaitSpeed(acceleration);
    float stepLength = calculateStepLength(acceleration);
    float stepTime = calculateStepTime();
    float stanceTime = calculateStanceTime();
    float swingTime = calculateSwingTime();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("C: ");
    lcd.print(cadence);
    lcd.print(" S: ");
    lcd.print(gaitSpeed);
    lcd.setCursor(0, 1);
    lcd.print("L: ");
    lcd.print(stepLength);
    lcd.print(" T: ");
    lcd.print(stepTime);

    delay(1000);

    interruptTriggered = false;

    dataFile = SD.open("gait_data.txt", FILE_WRITE);
    dataFile.print(cadence);
    dataFile.print(",");
    dataFile.print(gaitSpeed);
    dataFile.print(",");
    dataFile.print(stepLength);
    dataFile.print(",");
    dataFile.print(stepTime);
    dataFile.print(",");
    dataFile.print(stanceTime);
    dataFile.print(",");
    dataFile.print(swingTime);
    dataFile.println();
    dataFile.close();
  }
}

void interruptHandler() {
  interruptTime = millis();

  if (interruptTime - previousInterruptTime > 1000) {
    interruptTriggered = true;
    previousInterruptTime = interruptTime;
  }
}

float calculateCadence() {
  // TODO: Implement cadence calculation logic
}

float calculateGaitSpeed(float acceleration) {
  // TODO: Implement gait speed calculation logic
}

float calculateStepLength(float acceleration) {
  // TODO: Implement step length calculation logic
}

float calculateStepTime() {
  // TODO: Implement step time calculation logic
}

float calculateStanceTime() {
  // TODO: Implement stance time calculation logic
}

float calculateSwingTime() {
}

  // TODO
