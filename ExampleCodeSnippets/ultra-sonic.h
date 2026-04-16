class UltraSonic {
  private:
    const int trigPin = 2;  
    const int echoPin = 4; 
    float duration, distance;  
  public:
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


      Serial.print("Distance: ");  
      Serial.println(distance);  
      delay(100);  
    }
};

UltraSonic ultraSonic;
