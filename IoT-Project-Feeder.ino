#include "Arduino.h"
#include "ultra-sonic.h"
#include "water-monitor.h"
#include "StepperMotorCode.h"
#include "lcd-display.h"


StepperMotor feeder(0, 50, 12, 15);

void setup() {
    Serial.begin(9600);
    feeder.setup();
    ultraSonic.setup();
    lcdDisplay.setup();
    pinMode(16, OUTPUT);

    digitalWrite(16, HIGH);
}

void loop() {

  int buttonPressed = digitalRead(13);
  int moisture = waterMonitor.outputValue;
  float foodPercent = ultraSonic.distancePrecent;

  if (buttonPressed == LOW) {
    feeder.feed();
  }
  ultraSonic.loop();
  waterMonitor.loop();

  Serial.print("Percent Full: ");
  Serial.println(foodPercent);

  lcdDisplay.printMoisture(moisture, foodPercent);
  
  if (moisture > 50) {
    digitalWrite(16, HIGH);
  }
  else {
    digitalWrite(16, LOW);
  }

  delay(50);
}



