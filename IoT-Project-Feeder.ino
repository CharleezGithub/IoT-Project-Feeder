#include "Arduino.h"
#include "ultra-sonic.h"
#include "water-monitor.h"
#include "StepperMotorCode.h"


StepperMotor feeder(0, 50, 12, 15);

void setup() {
    Serial.begin(9600);
    feeder.setup();
    ultraSonic.setup();

}

void loop() {
  int buttonPressed = digitalRead(13);

  if (buttonPressed == LOW) {
    feeder.feed();
  }
  ultraSonic.loop();
  // waterMonitor.loop();

  Serial.println(ultraSonic.distancePrecent);
}
