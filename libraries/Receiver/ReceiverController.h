#include <Arduino.h>

class ReceiverController
{
public:
    ReceiverController();
    void setup();
    void loop();

private:
    const int receiverPin {0}; //Analog
    const int hallEffectSensorPin {D2}; //This is also interrupt 0
    const int powerLedPin {0};
    const int motorPowerLedPin {0};
    const int motorPowerRelayPin {0};
    const int motorPowerServoPin {0};

    const int signalLowTime {20};

    const unsigned int hallEffectSensorOnTimeLimit {1000}; //Number of micros the hall effect must be true to trigger
    unsigned int hallEffectSensorOnTime {0};
    bool motorPowerStatus {false};
    int motorPowerPreviousTimeValue {0};
    void UpdateMotorPowerStatusRising();
    void UpdateMotorPowerStatusFalling();

    unsigned int averagingListSize {10};
    unsigned int averagingList[averagingListSize];
    unsigned int averagingListIterator {0};
    void InitializeAveragingList(unsigned int initializerValue);
    bool CalculateMovingAverage(unsigned int rawSignal);
    void EmergencyBrake();
};
