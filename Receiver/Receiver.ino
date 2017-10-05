#include "ReceiverController.h"
ReceiverController receiver;

void setup() {  
  receiver.setup();

  cli();//stop interrupts

  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS22);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  sei();//allow interrupts
}

void loop() {
  receiver.loop();
  Serial.begin(9600);
}

ISR(TIMER2_COMPA_vect)
{//timer2 interrupt 8kHz toggles pin 9
    receiver.Interrupt();
}
