#include "TransmitterController.h"

TransmitterController::TransmitterController(){}

void TransmitterController::setup()
{
    // Set all IO
    pinMode(deadMansButtonPin, INPUT);
    pinMode(throttlePin, INPUT);
    pinMode(powerLedPin, OUTPUT);
    pinMode(transmitterPin, OUTPUT);

    // Set initial IO states
    digitalWrite(powerLedPin, HIGH);
    digitalWrite(transmitterPin, LOW);
}

void TransmitterController::loop()
{
    //Read Pot
    throttleReading = analogRead(throttlePin);

    //Send Data 
    if (digitalRead(deadMansButtonPin))
    {
        digitalWrite(transmitterPin, HIGH);
        delayMicroseconds(CalculateSignalHighTimeForRegularOperation());
        digitalWrite(transmitterPin, LOW);
        delay(signalLowTime);
    }
    else
    {
        throttleReading = 1023/2; //Half throttle should mean 0 speed
        const double emergencyStopThrottleIncrease {50}; //TODO Find right number
        digitalWrite(transmitterPin, HIGH);
        delayMicroseconds(CalculateSignalHighTime(emergencyStopThrottleIncrease));
        digitalWrite(transmitterPin, LOW);
        delay(signalLowTime);
    }
}

unsigned int TransmitterController::CalculateSignalHighTimeForRegularOperation()
{
    const double maximumThrottleIncreasePerSample = 15; //TODO Find right number
    return CalculateSignalHighTime(maximumThrottleIncreasePerSample);
}

unsigned int TransmitterController::CalculateSignalHighTime(double maximumThrottleIncreasePerSample)
{
    const unsigned int maximumThrottleValue = 1023;
    const unsigned int pwmBaseValue = 1000;
    const unsigned int deltaPwm = 1000;

    double deltaReading = throttleReading - previousThrottleReading;
    if (abs(deltaReading) > maximumThrottleIncreasePerSample)
    {
        if (deltaReading < 0)
        {
            deltaReading = -maximumThrottleIncreasePerSample;
        }
        else
        {
            deltaReading = maximumThrottleIncreasePerSample;
        }
    }

    throttleReading = previousThrottleReading + round(deltaReading);
    if (throttleReading < 0)
    {
        throttleReading = 0;
    }
    if (throttleReading > deltaPwm)
    {
        throttleReading = deltaPwm;
    }
    double pwm = ((throttleReading / maximumThrottleValue) * deltaPwm) + pwmBaseValue;
    previousThrottleReading = throttleReading;

    return round(pwm); //This is in micros
}

