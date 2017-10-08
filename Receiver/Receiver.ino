#include "ReceiverController.h"
ReceiverController receiver;
const int hallEffectSensorPin = 2; //This is also interrupt 0

void setup() {  
  receiver.setup();
  SetMotorPowerServoInterrupt();
  attachInterrupt(digitalPinToInterrupt(hallEffectSensorPin), HallEffectSensorTriggeredInterrupt, FALLING);
}

void loop() {
  receiver.loop();
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

//MotorPowerServoInterrupt code
void SetMotorPowerServoInterrupt()
{
  cli();//stop interrupts

  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 1khz increments
  OCR2A = 249;// = (16*10^6) / (1000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS22 bit for 64 prescaler
  TCCR2B |= (1 << CS22);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  sei();//allow interrupts
}

ISR(TIMER2_COMPA_vect)
{//timer2 interrupt 1kHz toggles pin 9
    receiver.UpdateMotorPowerServoInterrupt();
}
  
