#include <SpeedyStepper.h>

#define DIR_PIN  5   // GPIO5
#define STEP_PIN 4   // GPIO4

SpeedyStepper stepper;

int foodWeight = 0;
int foodWeightGoal = 150;
bool catFed = false;
int counter = 0;

void setup() {
  Serial.begin(115200);

  stepper.connectToPins(STEP_PIN, DIR_PIN);

  // Set motion parameters
  stepper.setStepsPerRevolution(1600);
  stepper.setSpeedInStepsPerSecond(800);
  stepper.setAccelerationInStepsPerSecondPerSecond(2000);

  Serial.println("Stepper ready");

  pinMode(12, INPUT_PULLUP);
}

void feed() {
  while (foodWeight <= foodWeightGoal) {
    if (foodWeight % 10 == 0) {
      Serial.print(foodWeight);
      Serial.println(" g has been given");
    }
    stepper.moveRelativeInSteps(50);
    foodWeight++;
    delay(5);
  }
  catFed = true;
  Serial.print("The cat's hunger has been satisfied... for now.");
}

void loop() {
  int buttonPressed = digitalRead(12);
  // Serial.println(buttonPressed);

  if (not catFed) {
    Serial.println("Hungry cat");
  }

  if (buttonPressed == LOW) {
    Serial.println("Feeding started");
    feed();
  }

  delay(50);
  // Move 1 revolution forward
  // stepper.moveRelativeInSteps(800);

  // Move 1 revolution backward
  // stepper.moveRelativeInSteps(-800);
  // delay(1000);
}