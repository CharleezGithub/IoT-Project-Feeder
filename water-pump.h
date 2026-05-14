class WaterPump
{
private:
    const int sensorPin = 16;

public:
    void loop()
    {
        int sensorValue = digitalRead(sensorPin);
        if (sensorValue == HIGH)
        // Serial.println(sensorValue);
        {
            int outputValue = map(sensorValue, 0, 650, 0, 255);
            Serial.println(outputValue);
        }
    }
};

WaterPump waterPump;
