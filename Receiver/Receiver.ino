#include "ReceiverController.h"
ReceiverController receiver;

void setup() {  
  receiver.setup();

  cli();//stop interrupts
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 159;// = (16*10^6) / (1*100*10^) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
}

void loop() {
  receiver.loop();
}

ISR(TIMER1_COMPA_vect)
{//timer1 interrupt 100kHz
  receiver.Interrupt();
}
