#include <Arduino.h>
#include <algorithm>

class UltraSonic {
  public:
    const int trigPin = 2;  
    const int echoPin = 4; 
    const float emptyDistance = 15.00f;
    const float minDistance = 4.50f;
    float duration, distance, distancePrecent;
    void setup() {
      pinMode(trigPin, OUTPUT);  
      pinMode(echoPin, INPUT);  
    }

    void loop() {  
      digitalWrite(trigPin, LOW);  
      delayMicroseconds(2);  
      digitalWrite(trigPin, HIGH);  
      delayMicroseconds(10);  
      digitalWrite(trigPin, LOW);

      duration = pulseIn(echoPin, HIGH);  
      distance = (duration*.0343)/2;  

      distancePrecent = std::min((distance - minDistance) / emptyDistance * 100, 100.00f);

      //Serial.print("Distance: ");
      //Serial.print();
      //Serial.println("%");
      //Serial.println(distance);
      delay(100);
    }
};

UltraSonic ultraSonic;
