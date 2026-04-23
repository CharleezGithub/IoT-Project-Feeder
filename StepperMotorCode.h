#include <SpeedyStepper.h>


SpeedyStepper stepper;

class StepperMotor {
  private:
  int foodWeight;
  int foodWeightGoal;
  int DIR_PIN;
  int STEP_PIN;
  
  
  public:
  StepperMotor(int weight, int max_weight, int dir_pin, int step_pin) {
    foodWeight = weight;
    foodWeightGoal = max_weight;
    DIR_PIN = dir_pin;   // GPIO5
    STEP_PIN = step_pin;   // GPIO4
  }

    bool catFed = false;
    int counter = 0;

  void setup() {
    Serial.begin(9600);

    stepper.connectToPins(STEP_PIN, DIR_PIN);

    // Set motion parameters
    stepper.setStepsPerRevolution(1600);
    stepper.setSpeedInStepsPerSecond(3200);
    stepper.setAccelerationInStepsPerSecondPerSecond(4000);

    Serial.println("Stepper ready");

    pinMode(12, INPUT_PULLUP);
  }

  void feed() {
    Serial.println("Feeding started");

    while (foodWeight <= foodWeightGoal) {
      if (foodWeight % 10 == 0) {
        Serial.print(foodWeight);
        Serial.println(" g has been given");
      }
      stepper.moveRelativeInSteps(-50);
      foodWeight++;
      delay(5);
    }
    catFed = true;
    Serial.print("The cat's hunger has been satisfied... for now.");
    foodWeight = 0;
  }

  void loop() {
    int buttonPressed = digitalRead(12);
    // Serial.println(buttonPressed);

    if (not catFed) {
      Serial.println("Hungry cat");
    }

    if (buttonPressed == LOW) {
      feed();
    }

    delay(50);
    // Move 1 revolution forward
    // stepper.moveRelativeInSteps(800);

    // Move 1 revolution backward
    // stepper.moveRelativeInSteps(-800);
    // delay(1000);
  }
};