#include <Wire.h>               // Arduino Library for I2C and other serial communication
#include "Adafruit_VEML7700.h"  // Driver library for VEML7700 light sensor

#define ENA_PIN   2                 // High enables stepper motor driver
#define DIR_PIN   3                 // Low is counter clockwise direction while High is clockwise
#define PUL_PIN   4                 // Alternate between High and Low to take single steps
#define NUM_ROTATIONS 18            // Number of rotations the motor will perform in open/close sequences
#define LIGHT_SENSOR_PERIOD_SEC 1   // Number of secinds that the Arudino will poll the VEML7700 light sensor
#define LUX_OPEN_THRESHOLD 800      // Lux level that will open the curtain when lux is above the given threshold
#define LUX_CLOSE_THRESHOLD 200     // Lux level that will close the curtain when lux is below the given threshold

/* Global variables/flags */
Adafruit_VEML7700 veml = Adafruit_VEML7700(); // Object that represents the VEML7700 light sensor
float lux = 0;                                // Float indicating the light level 
bool IsClosed = false;                        // Flag indicating open/close status

/* Function declarations */
void SetMotor();      // Sets the motor to the leftmost position during set up
void OpenMotor();     // Opens the curtain by moving the motor NUM_ROTATIONS in the counter clockwise direction
void CloseMotor();    // Closes the curtain by moving the motor NUM_ROTATIONS in the clockwise direction

void setup() {
  /* Initialize Serial BUS's */
  Serial.begin(9600);
  while (!Serial) delay(10);

  /* Initialize VEML7700 Light sensor */
  while (!veml.begin()) {
    Serial.println("VEML7700 not found. Check connections.");
    delay(1000);
  }
  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_100MS);

  /* Initialize all motor driver control pins */
  pinMode(PUL_PIN, OUTPUT);
  digitalWrite(PUL_PIN, LOW);  // Start Pulsing pin as Low
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW); // Starting Clockwise Direction
  pinMode(ENA_PIN, OUTPUT);
  digitalWrite(ENA_PIN, HIGH); // Turns on Stepper Motor Driver
  SetMotor();
}

void loop() {
  /* Read and update to current light level */
  lux = veml.readLux();
  while (lux == NAN) {
    Serial.println("VEML7700 not found. Check connections.");
    delay(1000);
    lux = veml.readLux();
  }
  Serial.print(lux);
  Serial.print("\n");
  
  /* Perform motor operations if needed */
  if(lux >= LUX_OPEN_THRESHOLD)
    OpenMotor();
  else if (lux <= LUX_CLOSE_THRESHOLD)
    CloseMotor();

  delay(LIGHT_SENSOR_PERIOD_SEC * 1000); // Wait for LIGHT_SENSOR_PERIOD_SEC seconds till next read 
}

/* Function Definitions */
// Motor Initialization Function
void SetMotor() {
  IsClosed = false;
  Serial.println("Setting the Curtain to open and setting starting position");
  digitalWrite(DIR_PIN, LOW);
  for (unsigned int CurrRotation = 0; CurrRotation < 20; CurrRotation++) {
    /* Performs 200 steps, which is equal to one rotation */
    for (unsigned int CurrStep = 0; CurrStep < 200; CurrStep++) {
      digitalWrite(PUL_PIN, LOW);
      delay(1);
      digitalWrite(PUL_PIN, HIGH);
      delay(1);
    }
  }
}

// Open Motor Function
void OpenMotor() {
  /* Check and update open/close state flag */
  if (IsClosed == false) // Avoids opening the curtain twice
    return;
  IsClosed = false;
  
  /* Perform NUM_ROTATION rotations*/
  Serial.println("Opening the Curtain");
  digitalWrite(DIR_PIN, LOW);
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
  if (IsClosed == true) // Avoids closing  the curtain twice
    return;
  IsClosed = true;

  /* Perform NUM_ROTATION rotations*/
  Serial.println("Closing the Curtain");
  digitalWrite(DIR_PIN, HIGH);
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