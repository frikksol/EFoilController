#include <Arduino.h>
#include <SoftwareSerial.h>

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
    const int motorPowerServoPin = 6;

    SoftwareSerial BTSerial = SoftwareSerial(bluetoothTxPin, bluetoothRxPin);
    void ReadNewThrottleValue();


    volatile bool signalIsHigh = false;
    volatile double numberOfTicksSinceLastFlank = 0;
    const double numberOfMicrosPerTick = 10;
    const double signalLowTime = 20000;
    volatile double signalHighTime = 0;

    const int pwmBaseValue = 1000;
    const int pwmMidValue = pwmBaseValue + 500;
    const int pwmMaxValue = pwmMidValue + 500;


    const unsigned int hallEffectSensorOnTimeLimit = 1000; //Number of micros the hall effect must be true to trigger
    unsigned int hallEffectSensorOnTime = 0;
    volatile bool motorPowerStatus = false;
    volatile int motorPowerPreviousTimeValue = 0;
};
