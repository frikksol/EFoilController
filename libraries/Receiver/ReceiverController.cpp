#include "ReceiverController.h"

ReceiverController::ReceiverController(){}

void ReceiverController::setup()
{
    // Set all IO
    pinMode(hallEffectSensorPin, INPUT);
    pinMode(powerLedPin, OUTPUT);
    pinMode(motorPowerLedPin, OUTPUT);
    pinMode(motorPowerRelayPin, OUTPUT);
    motorPowerServo.attach(motorPowerServoPin);

    //Set inital values for outputs
    digitalWrite(powerLedPin, HIGH);
    digitalWrite(motorPowerLedPin, LOW);
    digitalWrite(motorPowerRelayPin, LOW);
    digitalWrite(motorPowerServoPin, LOW);

    //Start BlueTooth communication
    BTSerial.begin(38400);

    //TEMP
    motorPowerStatus = true; // TODO remove this when magnet switch is added
}

void ReceiverController::loop()
{
    if (motorPowerStatus)
    {
        ReadNewThrottleValue();
        digitalWrite(motorPowerLedPin, HIGH);
    }
    else
    {
        digitalWrite(motorPowerLedPin, LOW);
    }

}

void ReceiverController::ReadNewThrottleValue()
{
    const char messageStart = 'S';
    const char messageEnd = 'E';
    String receivedString = "";
    while (BTSerial.available() > 0)
    {
        int receivedChar = BTSerial.read();
        if (receivedChar == messageStart)
        {
            receivedString = "";
        }
        else if (isDigit(receivedChar))
        {
            receivedString += (char)receivedChar;
        }
        else if (receivedChar == messageEnd)
        {
            //TODO Extract to function!
            const double minimumValue = 100;
            const double middleValue = 510;
            const double maximumValue = 900;
            const double deadband = 30;

            int received = receivedString.toDouble();
            if (received < minimumValue)
            {
                break;
            }
            if ((received > middleValue - deadband) && (received < middleValue + deadband))
            {
                received = middleValue;
            }
            if (received > maximumValue)
            {
                break;
            }

            double receivedConverted = ((received - minimumValue) / (maximumValue - minimumValue)) * 180;
            int previousPosition = servoPosition;
            int position = (int) round(receivedConverted);
            servoPosition = position;

            receivedString = "";
        }
    }
}

void ReceiverController::Interrupt()
{
    motorPowerServo.write(servoPosition);
}
