#include <Arduino.h>
#include <SoftwareSerial.h>

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
    const int bluetoothTxPin = 10;
    const int bluetoothRxPin = 11;

    unsigned int throttleReading = 0;
    void SendThrottleReading();
    void SendThrottleReading(unsigned int reading);
    SoftwareSerial BTSerial = SoftwareSerial(bluetoothTxPin, bluetoothRxPin);
};
