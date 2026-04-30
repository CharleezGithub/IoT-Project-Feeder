class WaterMonitor
{
private:
    const int sensorPin = A0;
    float duration, distance;

public:
    void loop()
    {
        int sensorValue = analogRead(sensorPin);
        if (sensorValue > 0)
        // Serial.println(sensorValue);
        {
            int outputValue = map(sensorValue, 0, 650, 0, 255);
            Serial.println(outputValue);
        }
    }
};

WaterMonitor waterMonitor;
