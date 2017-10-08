#include "TransmitterController.h"

TransmitterController::TransmitterController()
{
}

void TransmitterController::setup()
{
    // Set all IO
    pinMode(deadMansButtonPin, INPUT);
    pinMode(throttlePin, INPUT);
    pinMode(powerLedPin, OUTPUT);

    // Set initial IO states
    digitalWrite(powerLedPin, HIGH);

    //Start BlueTooth communication
    BTSerial.begin(9600);
}

void TransmitterController::loop()
{
    //Read Pot
    throttleReading = analogRead(throttlePin);

    //Send Data 
    if (digitalRead(deadMansButtonPin))
    {
        SendThrottleReading();
    }
    else
    {
        const int zeroSpeed = 510;
        SendThrottleReading(zeroSpeed);
    }
}

void TransmitterController::SendThrottleReading()
{
    SendThrottleReading(throttleReading);
}

void TransmitterController::SendThrottleReading(unsigned int reading)
{
    const char messageStart = 'S';
    const char messageEnd = 'E';
    String messageBuffer = String(reading);

    delay(100); //TODO Do this with interrupt instead

    BTSerial.write(messageStart);
    for (int character = 0; character < messageBuffer.length(); character++)
    {
        BTSerial.write(messageBuffer[character]);
    }
    BTSerial.write(messageEnd);
}
