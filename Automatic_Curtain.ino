#define PUL_PIN   2 // Alternate between High and Low to take single steps
#define DIR_PIN   3 // Low is counter clockwise direction while High is clockwise
#define ENA_PIN   4 // High enables stepper motor driver

void setup() {
  // Initialize all motor driver control pins
  pinMode(PUL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);

  // Set all motor driver control pins
  digitalWrite(PUL_PIN, LOW);  // Start Pulsing pin as Low
  digitalWrite(DIR_PIN, HIGH); // Starting Clockwise Direction
  digitalWrite(ENA_PIN, HIGH); // Turns on Stepper Motor Driver
}

void loop() {
  // Perform two full steps
  digitalWrite(PUL_PIN, LOW);
  delay(1);
  digitalWrite(PUL_PIN, HIGH);
  delay(1);
}
