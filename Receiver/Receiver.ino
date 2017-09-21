#include "ReceiverController.h"
ReceiverController receiver;


void setup() {
  // put your setup code here, to run once:
  receiver.setup();
  attachInterrupt(0, RisingInterrupt, RISING);

}

void loop() {
  // put your main code here, to run repeatedly:
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
