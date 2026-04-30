#include "Arduino.h"
#include "ultra-sonic.h"
#include "water-monitor.h"
#include "StepperMotorCode.h"


StepperMotor feeder(0, 50, 12, 15);

void setup() {
    Serial.begin(9600);
    feeder.setup();
    ultraSonic.setup();
    pinMode(16, OUTPUT);

    digitalWrite(16, LOW);
}

void loop() {

  int buttonPressed = digitalRead(13);

  if (buttonPressed == LOW) {
    feeder.feed();
  }
  ultraSonic.loop();
  waterMonitor.loop();


  Serial.print("Percent Full: ");
  Serial.println(ultraSonic.distancePrecent);

  if (waterMonitor.outputValue < 50) {
    digitalWrite(16, HIGH);
  }
  else {
    digitalWrite(16, LOW);
  }

  delay(50);
}
