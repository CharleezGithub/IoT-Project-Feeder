#include <Arduino.h>
#include <ultra-sonic.h>

extern UltraSonic ultraSonic;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);

    ultraSonic.setup();

}

void loop() {
  // put your main code here, to run repeatedly:
  ultraSonic.loop();
}
