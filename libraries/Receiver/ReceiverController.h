#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Servo.h>

class ReceiverController
{
public:
    ReceiverController();
    void setup();
    void loop();
    void Interrupt();

private:
    const int bluetoothTxPin = 10;
    const int bluetoothRxPin = 11;
    const int hallEffectSensorPin = 2; //This is also interrupt 0
    const int powerLedPin = 3;
    const int motorPowerLedPin = 4;
    const int motorPowerRelayPin = 5;
    const int motorPowerServoPin = 9;

    SoftwareSerial BTSerial = SoftwareSerial(bluetoothTxPin, bluetoothRxPin);
    void ReadNewThrottleValue();

    Servo motorPowerServo;
    volatile int servoPosition = 90;

    const unsigned int hallEffectSensorOnTimeLimit = 1000; //Number of micros the hall effect must be true to trigger
    unsigned int hallEffectSensorOnTime = 0;
    volatile bool motorPowerStatus = false;
};
