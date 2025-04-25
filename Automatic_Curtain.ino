#include <Wire.h>               // Arduino Library for I2C and other serial communication
#include <Adafruit_VEML7700.h>  // Driver library for VEML7700 light sensor

#define PUL_PIN   2                 // Alternate between High and Low to take single steps
#define DIR_PIN   3                 // Low is counter clockwise direction while High is clockwise
#define ENA_PIN   4                 // High enables stepper motor driver
#define NUM_ROTATIONS 15            // Number of rotations the motor will perform in open/close sequences
#define LIGHT_SENSOR_PERIOD_SEC 60  // Number of secinds that the Arudino will poll the VEML7700 light sensor
#define LUX_OPEN_THRESHOLD 1000     // Lux level that will open the curtain when lux is above the given threshold
#define LUX_CLOSE_THRESHOLD 700     // Lux level that will close the curtain when lux is below the given threshold

/* Global variables/flags */
Adafruit_VEML7700 veml = Adafruit_VEML7700(); // Object that represents the VEML7700 light sensor
float lux = 0;
bool IsClosed = false;                        // Flag indicating open/close status

/* Function declarations */
void OpenMotor();
void CloseMotor();

void setup() {
  /* Initialize all motor driver control pins */
  pinMode(PUL_PIN, OUTPUT);
  digitalWrite(PUL_PIN, LOW);  // Start Pulsing pin as Low
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW); // Starting Clockwise Direction
  pinMode(ENA_PIN, OUTPUT);
  digitalWrite(ENA_PIN, HIGH); // Turns on Stepper Motor Driver

  /* Initialize VEML7700 Light sensor */
  Serial.begin(9600);
  while (!Serial) delay(10);

  if (!veml.begin()) {
    Serial.println("VEML7700 not found. Check connections.");
    while (1) delay(10);
  }

  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_100MS);
}

void loop() {
  /* Read and update to current light level */
  lux = veml.readLux();
  Serial.print(lux);

  /* Perform motor operations if needed */
  if (lux >= LUX_OPEN_THRESHOLD) 
    Serial.print("OPENED");
  else if (lux <= LUX_CLOSE_THRESHOLD)
    Serial.print("CLOSED");

  delay(LIGHT_SENSOR_PERIOD_SEC * 1000); // Wait for LIGHT_SENSOR_PERIOD_SEC seconds till next read 
}

/* Function Definitions */
// Open Motor Function
void OpenMotor() {
  /* Update open/close state flag */
  IsClosed = false;
  digitalWrite(DIR_PIN, LOW);

  /* Perform NUM_ROTATION rotations*/
  for (unsigned int CurrRotation = 0; CurrRotation < NUM_ROTATIONS; CurrRotation++) {
    /* Performs 200 steps, which is equal to one rotation */
    for (unsigned int CurrStep = 0; CurrStep < 200; CurrStep++) {
      digitalWrite(PUL_PIN, LOW);
      delay(1);
      digitalWrite(PUL_PIN, HIGH);
      delay(1);
    }
  }
}

// Close Motor Function
void CloseMotor() {
  /* Update open/close state flag */
  IsClosed = true;
  digitalWrite(DIR_PIN, HIGH);

  /* Perform NUM_ROTATION rotations*/
  for (unsigned int CurrRotation = 0; CurrRotation < NUM_ROTATIONS; CurrRotation++) {
    /* Performs 200 steps, which is equal to one rotation */
    for (unsigned int CurrStep = 0; CurrStep < 200; CurrStep++) {
      digitalWrite(PUL_PIN, LOW);
      delay(1);
      digitalWrite(PUL_PIN, HIGH);
      delay(1);
    }
  }
}