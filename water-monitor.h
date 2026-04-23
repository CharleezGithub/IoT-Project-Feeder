class WaterMonitor
{
private:
    const int sensorPin = 36;
    float duration, distance;

public:
    void loop()
    {
        int sensorValue = analogRead(sensorPin);
        if (sensorValue > 570)
        {
            int outputValue = map(sensorValue, 570, 800, 0, 255);
            Serial.println(outputValue);
        }
    }
};

WaterMonitor waterMonitor;
