#include "ultra-sonic.h"
#include "water-monitor.h"
#include "StepperMotorCode.h"
#include "lcd-display.h"

StepperMotor feeder(0, 50, 12, 15);
int targetFoodAmount = 0; // Received from ESP32

void setup() {
    Serial.begin(9600); // Default Hardware Serial
    feeder.setup();
    ultraSonic.setup();
    lcdDisplay.setup();
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
}

void loop() {
  // 1. Receive data from ESP32
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    
    // Parse target food amount
    int foodPos = command.indexOf("SETFOOD:");
    if (foodPos != -1) {
      targetFoodAmount = command.substring(foodPos + 8).toInt();
    }

    // New: Parse the manual Feed command
    if (command.indexOf("FEED:1") != -1) {
      feeder.feed(); // Trigger the motor
    }
  }

  // 2. Existing Sensor Logic
  ultraSonic.loop();
  waterMonitor.loop();
  
  int moisture = waterMonitor.outputValue;
  float foodPercent = ultraSonic.distancePrecent;

  // 3. Send sensor data back to ESP32 for the Web Dashboard
  // Format: W:[moisture],F:[foodPercent]
  Serial.print("W:");
  Serial.print(moisture);
  Serial.print(",F:");
  Serial.println(foodPercent);

  // 4. Feeder Logic
  if (digitalRead(13) == LOW) {
    feeder.feed();
  }

  // Visuals and Controls
  lcdDisplay.printMoisture(moisture, foodPercent);
  digitalWrite(10, (moisture > 50) ? HIGH : LOW);

  delay(100); 
}