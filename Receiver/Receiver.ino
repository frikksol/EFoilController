#include "ReceiverController.h"
ReceiverController receiver;


void setup() {
  receiver.setup();
  attachInterrupt(0, RisingInterrupt, RISING);
}

void loop() {
  receiver.loop();
}

void RisingInterrupt()
{
  receiver.UpdateMotorPowerStatusRising();
  attachInterrupt(0, FallingInterrupt, FALLING);
}

void FallingInterrupt()
{
  receiver.UpdateMotorPowerStatusFalling();
  attachInterrupt(0, RisingInterrupt, RISING);
}
