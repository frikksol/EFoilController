#include <Arduino.h>
#include <Servo.h>

class ReceiverController
{
public:
    ReceiverController();
    void setup();
    void ReadNewThrottleValueInterrupt();
    void UpdateMotorPowerServoInterrupt();
    void HallEffectSensorTriggeredInterrupt();
    void HallEffectSensorUntriggeredInterrupt();

private:
    const int powerLedPin = 6;
    const int motorPowerLedPin = 4;
    const int motorPowerRelayPin = 5;
    const int motorPowerServoPin = 9;

    void ReadNewThrottleValue();
    int ConvertFromBluetoothStringToInt(String btString);
    int LinearizeValue(int previousValue, int newValue);

    Servo motorPowerServo;
    volatile int servoPosition = 90;

    const unsigned long hallEffectSensorOnTimeLimit = 500; //Number of micros the hall effect must be true to trigger
    volatile unsigned long hallEffectSensorOnTime = 0;
    volatile bool motorPowerStatus = false;

    unsigned long timeSinceLastBtReading = 0;
    unsigned long btReadingTimeout = 1000;
};
