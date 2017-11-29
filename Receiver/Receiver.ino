#include "ReceiverController.h"
ReceiverController receiver;
const int hallEffectSensorPin = 2; //This is also interrupt 0
const int outputTriggerPin = 3;

void setup() {
    receiver.setup();
    attachInterrupt(digitalPinToInterrupt(hallEffectSensorPin), HallEffectSensorTriggeredInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(outputTriggerPin), outputTriggerPinFalling, FALLING);
}

void loop() {
    receiver.UpdateMotorPowerServoInterrupt();
}

void outputTriggerPinFalling()
{
    receiver.ReadNewThrottleValueInterrupt();

}

//HallEffectSensorInterrupt code
void HallEffectSensorTriggeredInterrupt()
{
    receiver.HallEffectSensorTriggeredInterrupt();
    attachInterrupt(digitalPinToInterrupt(hallEffectSensorPin), HallEffectSensorUntriggeredInterrupt, RISING);

}
void HallEffectSensorUntriggeredInterrupt()
{
    receiver.HallEffectSensorUntriggeredInterrupt();
    attachInterrupt(digitalPinToInterrupt(hallEffectSensorPin), HallEffectSensorTriggeredInterrupt, FALLING);
}
