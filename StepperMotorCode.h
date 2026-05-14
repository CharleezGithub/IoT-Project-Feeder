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
    DIR_PIN = dir_pin;
    STEP_PIN = step_pin;
  }

    bool catFed = false;
    int counter = 0;

  void setup() {
    Serial.begin(9600);

    stepper.connectToPins(STEP_PIN, DIR_PIN);

    // Set motion parameters
    stepper.setStepsPerRevolution(1600);
    stepper.setSpeedInStepsPerSecond(1000);
    stepper.setAccelerationInStepsPerSecondPerSecond(10000);

    Serial.println("Stepper ready");

    pinMode(13, INPUT_PULLUP);
  }

  void feed() {
    Serial.println("Feeding started");

    // while (foodWeight <= foodWeightGoal) {
    //   if (foodWeight % 10 == 0) {
    //     Serial.print(foodWeight);
    //     Serial.println(" g has been given");
    //   }
    //   stepper.moveRelativeInSteps(50);
    //   foodWeight++;
    //   delay(1);
    
    // catFed = true;
    // Serial.print("The cat's hunger has been satisfied... for now.");
    // foodWeight = 0;
    stepper.moveRelativeInRevolutions(-0.5);
    Serial.println("Feeding finished");
  }

  void loop() {
    int buttonPressed = digitalRead(12);

    if (buttonPressed == LOW) {
      feed();
    }

    delay(50);
  }
};