#include <Arduino.h>

class ReceiverController
{
public:
    ReceiverController();
    void setup();
    void loop();
    void UpdateMotorPowerStatusRising();
    void UpdateMotorPowerStatusFalling();

private:
    const int receiverPin = A0; //Analog
    const int hallEffectSensorPin = 2; //This is also interrupt 0
    const int powerLedPin = 3;
    const int motorPowerLedPin = 4;
    const int motorPowerRelayPin = 5;
    const int motorPowerServoPin = 6;

    const unsigned int hallEffectSensorOnTimeLimit = 1000; //Number of micros the hall effect must be true to trigger
    unsigned int hallEffectSensorOnTime = 0;
    volatile bool motorPowerStatus = false;
    volatile int motorPowerPreviousTimeValue = 0;

    int averagingListSize = 10;
    unsigned int averagingList[10];
    unsigned int averagingListIterator = 0;
    void InitializeAveragingList(unsigned int initializerValue);
    bool CalculateMovingAverage(unsigned int rawSignal);
    void EmergencyBrake();
};
