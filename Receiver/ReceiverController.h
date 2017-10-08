#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Servo.h>

class ReceiverController
{
public:
    ReceiverController();
    void setup();
    void loop();
    void UpdateMotorPowerServoInterrupt();
    void HallEffectSensorTriggeredInterrupt();
    void HallEffectSensorUntriggeredInterrupt();

private:
    const int bluetoothTxPin = 10;
    const int bluetoothRxPin = 11;
    const int powerLedPin = 3;
    const int motorPowerLedPin = 4;
    const int motorPowerRelayPin = 5;
    const int motorPowerServoPin = 9;

    SoftwareSerial BTSerial = SoftwareSerial(bluetoothTxPin, bluetoothRxPin);
    void ReadNewThrottleValue();
    int ConvertFromBluetoothStringToInt(String btString);
    int LinearizeValue(int previousValue, int newValue);

    Servo motorPowerServo;
    volatile int servoPosition = 90;

    const unsigned long hallEffectSensorOnTimeLimit = 500; //Number of micros the hall effect must be true to trigger
    volatile unsigned long hallEffectSensorOnTime = 0;
    volatile bool motorPowerStatus = false;
};
