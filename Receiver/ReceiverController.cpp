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
    BTSerial.begin(9600);
    
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
            int position = ConvertFromBluetoothStringToInt(receivedString);
            int previousPosition = servoPosition;
            int newPosition = LinearizeValue(previousPosition, position);

            servoPosition = newPosition;
            receivedString = "";
        }
    }
}

int ReceiverController::ConvertFromBluetoothStringToInt(String btString)
{
    const double minimumBtValue = 100;
    const double maximumBtValue = 900;
    const double maximumIntValue = 180;
    int received = btString.toDouble();
    double receivedConverted = ((received - minimumBtValue) / (maximumBtValue - minimumBtValue)) * maximumIntValue;
    return (int) round(receivedConverted);
}

int ReceiverController::LinearizeValue(int previousValue, int newValue)
{
    const int maximiumIncrease = 10;
    const int deadband = 10;
    const int minimumValue = 0;
    const int middleValue = 90;
    const int maximumValue = 180;

    //Adjustment as input is shifted wrong
    newValue -= 10;

    //Adjusting the value for limits
    if (newValue < minimumValue + 2*deadband)
    {
        newValue = minimumValue;
    }
    if (newValue > maximumValue - 2*deadband)
    {
        newValue = maximumValue;
    }
    if((newValue > middleValue - deadband) && (newValue < middleValue + deadband))
    {
        newValue = middleValue;
    }

    //Limiting the increase to be within linearization maximums
    int increase = newValue - previousValue;

    if (abs(increase) > maximiumIncrease)
    {
        if (increase < 0)
        {
            increase = -maximiumIncrease;
        }
        else
        {
            increase = maximiumIncrease;
        }
    }

    //Creating the value to return
    int returnValue = previousValue + increase;
    if (returnValue < minimumValue)
    {
        returnValue = minimumValue;
    }
    if (returnValue > maximumValue)
    {
        returnValue = maximumValue;
    }

    return returnValue;
}

void ReceiverController::Interrupt()
{
    int position = servoPosition;
    motorPowerServo.write(position);
}