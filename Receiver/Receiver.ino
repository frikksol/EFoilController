#include "ReceiverController.h"
ReceiverController receiver;
const int hallEffectSensorPin = 2; //This is also interrupt 0
const int lolPin = 3;

void setup() {
    receiver.setup();
    attachInterrupt(digitalPinToInterrupt(hallEffectSensorPin), HallEffectSensorTriggeredInterrupt, FALLING);
    attachInterrupt(digitalPinToInterrupt(lolPin), lol, FALLING);

}

void loop() {
    receiver.UpdateMotorPowerServoInterrupt();
}

void lol()
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
