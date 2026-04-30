class WaterMonitor
{
private:
    const int sensorPin = A0;
    float duration, distance;

public:
    int outputValue;

    void loop()
    {
        int sensorValue = analogRead(sensorPin);
        if (sensorValue > 0)
        // Serial.println(sensorValue);
        {
            outputValue = map(sensorValue, 0, 650, 0, 255);
            Serial.print("Humidity: ");
            Serial.println(outputValue);
        }
    }
};

WaterMonitor waterMonitor;
