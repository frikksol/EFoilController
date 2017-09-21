#include <Arduino.h>

class TransmitterController
{
public:
    TransmitterController();
    void setup();
    void loop();

private:
    const int powerLedPin = 3;
    const int deadMansButtonPin = 4;
    const int throttlePin = A1; //Analog
    const int transmitterPin = A0;

    const int signalLowTime = 20; //This is in millis

    unsigned int throttleReading = 0;
    unsigned int previousThrottleReading = 0;
    unsigned int CalculateSignalHighTimeForRegularOperation();
    unsigned int CalculateSignalHighTime(double maximumThrottleIncreasePerSample);
};
