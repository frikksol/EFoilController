#include "ReceiverController.h"

ReceiverController::ReceiverController(){}

void ReceiverController::setup()
{
    // Set all IO
    pinMode(hallEffectSensorPin, INPUT);
    pinMode(powerLedPin, OUTPUT);
    pinMode(motorPowerLedPin, OUTPUT);
    pinMode(motorPowerRelayPin, OUTPUT);
    pinMode(motorPowerServoPin, OUTPUT);

    //Set inital values for outputs
    digitalWrite(powerLedPin, HIGH);
    digitalWrite(motorPowerLedPin, LOW);
    digitalWrite(motorPowerRelayPin, LOW);
    digitalWrite(motorPowerServoPin, LOW);

    //Start BlueTooth communication
    BTSerial.begin(38400);

    //TEMP
    motorPowerStatus = true;
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
        signalHighTime = 0;
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
                //received = minimumValue;
            }
            if ((received > middleValue - deadband) && (received < middleValue + deadband))
            {
                received = middleValue;
            }
            if (received > maximumValue)
            {
                break;
                //received = maximumValue;
            }

            double receivedConverted = (received - minimumValue) / (maximumValue - minimumValue) * pwmBaseValue;
            double currentPwm = signalHighTime - pwmBaseValue;
            double increase = receivedConverted - currentPwm;
            const double increaseLimit = 2;

            if (abs(increase) > increaseLimit)
            {
                if (increase < 0)
                {
                    currentPwm -= increaseLimit;
                }
                else
                {
                    currentPwm += increaseLimit;
                }
            }
            else
            {
                currentPwm += increase;
            }

            if (currentPwm < 0)
            {
                currentPwm = 0;
            }
            if (currentPwm > pwmBaseValue)
            {
                currentPwm = pwmBaseValue;
            }
            signalHighTime = currentPwm + pwmBaseValue;

            receivedString = "";
        }
    }
}

void ReceiverController::Interrupt()
{
    numberOfTicksSinceLastFlank++;
    double currentWait = numberOfTicksSinceLastFlank * numberOfMicrosPerTick;
    if (signalIsHigh && (currentWait >= signalHighTime))
    {
        digitalWrite(motorPowerServoPin, LOW);
        signalIsHigh = false;
        numberOfTicksSinceLastFlank = 0;
    }
    else if (!signalIsHigh && (currentWait >= signalLowTime))
    {
        digitalWrite(motorPowerServoPin, HIGH);
        signalIsHigh = true;
        numberOfTicksSinceLastFlank = 0;
    }
}
