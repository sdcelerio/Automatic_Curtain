#include <Adafruit_VEML7700.h>

#define PUL_PIN   2       // Alternate between High and Low to take single steps
#define DIR_PIN   3       // Low is counter clockwise direction while High is clockwise
#define ENA_PIN   4       // High enables stepper motor driver
#define NUM_ROTATIONS 15  // Number of rotations the motor will perform in open/close sequences

bool IsClosed = false;

/* Open Motor Function */
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

/* Close Motor Function */
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

void setup() {
  /* Initialize all motor driver control pins */
  pinMode(PUL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);

  /* Set all motor driver control pins */
  digitalWrite(PUL_PIN, LOW);  // Start Pulsing pin as Low
  digitalWrite(DIR_PIN, LOW); // Starting Clockwise Direction
  digitalWrite(ENA_PIN, HIGH); // Turns on Stepper Motor Driver
}

void loop() {
  CloseMotor();
  delay(1000);

  OpenMotor();
  delay(1000);
}
