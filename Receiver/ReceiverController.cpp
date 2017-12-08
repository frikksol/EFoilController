#include "ReceiverController.h"

ReceiverController::ReceiverController(){}

void ReceiverController::setup()
{
    // Set all IO
    pinMode(powerLedPin, OUTPUT);
    pinMode(motorPowerLedPin, OUTPUT);
    pinMode(motorPowerRelayPin, OUTPUT);
    motorPowerServo.attach(motorPowerServoPin);

    //Set inital values for outputs
    digitalWrite(powerLedPin, HIGH);
    digitalWrite(motorPowerLedPin, LOW);
    digitalWrite(motorPowerRelayPin, LOW);

    //Start Bluetooth communication
    Serial.begin(9600);

    //Temp
    motorPowerStatus = true;
}

void ReceiverController::ReadNewThrottleValueInterrupt()
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

    if (Serial.available() >=  3)
    {
      while (Serial.available() > 0)
      {
        int receivedChar = Serial.read();
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

            if (newPosition >= 90)
            {
                 servoPosition = newPosition;
            }
            
            receivedString = "";
            timeSinceLastBtReading = millis();
        }
      }
    }
    
    
    if (millis() - timeSinceLastBtReading > btReadingTimeout)
    {
        int previousPosition = servoPosition;
        int newPosition = LinearizeValue(previousPosition, 90);
        servoPosition = newPosition;
    }
}

int ReceiverController::ConvertFromBluetoothStringToInt(String btString)
{
    const int minimumBtValue = 1;
    const int maximumBtValue = 1000;
    const int minimumIntValue = 90;
    const int maximumIntValue = 180;
    int received = btString.toInt();
    
    int returnValue = map(received, minimumBtValue, maximumBtValue, minimumIntValue, maximumIntValue);

    if(returnValue < minimumIntValue)
    {
      returnValue = minimumIntValue;
    }

    
    return returnValue;
}

int ReceiverController::LinearizeValue(int previousValue, int newValue)
{
    //Creating the value to return
    int returnValue = 0;
    int diff = abs(previousValue - newValue);
    int diffMax = 20;
    
    if( (diff > diffMax) && !PreviousPositionMemory)
    {
      returnValue = previousValue;
      PreviousPositionMemory = true;
    }
    else
    {
      returnValue = newValue;
      PreviousPositionMemory = false;
    }  
    return returnValue;
}

void ReceiverController::UpdateMotorPowerServoInterrupt()
{
    int position = servoPosition;
    if (servoPosition > 5)
    {
      motorPowerServo.write(position);
    }
}

void ReceiverController::HallEffectSensorTriggeredInterrupt()
{
    hallEffectSensorOnTime = millis();
}

void ReceiverController::HallEffectSensorUntriggeredInterrupt()
{
    unsigned long elapsed = millis() - hallEffectSensorOnTime;
    if (elapsed > hallEffectSensorOnTimeLimit)
    {
        if (motorPowerStatus)
        {
            motorPowerStatus = false;
            digitalWrite(motorPowerLedPin, LOW);
            digitalWrite(motorPowerRelayPin, LOW);
        }
        else
        {
            motorPowerStatus = true;
            digitalWrite(motorPowerLedPin, HIGH);
            digitalWrite(motorPowerRelayPin, HIGH);
        }
    }
}