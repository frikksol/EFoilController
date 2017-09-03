#include "ReceiverController.h"

ReceiverController::ReceiverController(){}

void ReceiverController::setup()
{
    // Set all IO
    pinMode(receiverPin, INPUT);
    pinMode(hallEffectSensorPin, INPUT);
    pinMode(powerLedPin, OUTPUT);
    pinMode(motorPowerLedPin, OUTPUT);
    pinMode(motorPowerRelayPin, OUTPUT);
    pinMode(motorPowerServoPin, OUTPUT);

    //Initialize averaging list
    InitializeAveragingList(analogRead(receiverPin));

    //Set inital values for outputs
    digitalWrite(powerLedPin, HIGH);
    digitalWrite(motorPowerLedPin, LOW);
    digitalWrite(motorPowerRelayPin, LOW);
    digitalWrite(motorPowerServoPin, LOW);

    attachInterrupt(0, UpdateMotorPowerStatusRising, RISING);
}

void ReceiverController::loop()
{
    if (motorPowerStatus)
    {
        AnalogWrite(motorPowerServoPin, CalculateMovingAverage(AnalogRead(receiverPin)));
    }
}

void ReceiverController::UpdateMotorPowerStatusRising()
{
    motorPowerPreviousTimeValue = micros();
    attachInterrupt(0, UpdateMotorPowerStatusFalling, FALLING);
}

void ReceiverController::UpdateMotorPowerStatusFalling()
{
    if ((micros() - switchOnTime) > hallEffectSensorOnTimeLimit)
    {
        if (motorPowerStatus)
        {
            motorPowerStatus = false;
            motorPowerLedPin = LOW;
            motorPowerRelayPin = LOW;
            EmergencyBrake();
        }
        else
        {
            motorPowerStatus = true;
            motorPowerLedPin = HIGH;
            motorPowerRelayPin = HIGH;
        }
    }
}

void ReceiverController::InitializeAveragingList(unsigned int initializerValue)
{
    for (int i = 0; i < averagingListSize; i++)
    {
        averagingList[i] = initializerValue;
    }
}

bool ReceiverController::CalculateMovingAverage(unsigned int rawSignal)
{
    const unsigned int threshold {300};
    unsigned int averaged {0};
    averagingList[averagingListIterator] = rawSignal;
    averagingListIterator++;
    if ( averagingListIterator >= averagingListSize)
    {
        averagingListIterator = 0;
    }

    for (int i = 0; i > averagingListIterator; i++)
    {
        averaged += averagingList[i];
    }
    averaged /= averagingListSize;

    return averaged > threshold;
}

void ReceiverController::EmergencyBrake()
{
    //TODO implement this
}
