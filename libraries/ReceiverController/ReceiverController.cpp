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

    //TEMP
    motorPowerStatus = true;
}

void ReceiverController::loop()
{
    if (motorPowerStatus)
    {
        digitalWrite(motorPowerServoPin, CalculateMovingAverage(analogRead(receiverPin)));
    }
}

void ReceiverController::UpdateMotorPowerStatusRising()
{
    motorPowerPreviousTimeValue = micros();
}

void ReceiverController::UpdateMotorPowerStatusFalling()
{
    if ((micros() - motorPowerPreviousTimeValue) > hallEffectSensorOnTimeLimit)
    {
        if (motorPowerStatus)
        {
            motorPowerStatus = false;
            digitalWrite(motorPowerLedPin, LOW);
            digitalWrite(motorPowerRelayPin, LOW);
            EmergencyBrake();
        }
        else
        {
            motorPowerStatus = true;
            digitalWrite(motorPowerLedPin, HIGH);
            digitalWrite(motorPowerRelayPin, HIGH);
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
