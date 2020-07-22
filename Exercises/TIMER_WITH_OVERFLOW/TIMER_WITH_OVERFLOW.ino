#include <avr/io.h>

void setup() {
  DDRB |=(1<<PB5); 
 
 noInterrupts();
 
 TCCR1A=0;    


 TCNT1= 15625;
 TCCR1B |=(1<<CS12);      // sets the prescalar to 1024
TCCR1B &=~(1<<CS11);
TCCR1B |=(1<<CS10);
TIMSK1 |=(1<<TOIE1);
interrupts();
}
ISR(TIMER1_OVF_vect){
TCNT1=15625;
 PORTB ^=(1<<PB5);;
  
}
void loop() {
  // put your main code here, to run repeatedly:

}


/*sourcecode language=”cpp”]
/*
* timer and interrupts
* Timer1 overflow interrupt example
* more infos: https://oscarliang.com


#define ledPin 13

void setup()
{
pinMode(ledPin, OUTPUT);

// initialize Timer1
noInterrupts(); // disable all interrupts
TCCR1A = 0;
TCCR1B = 0;

TCNT1 = 34286; // preload timer 65536-16MHz/256/2Hz
TCCR1B |= (1 &lt;&lt; CS12); // 256 prescaler
TIMSK1 |= (1 &lt;&lt; TOIE1); // enable timer overflow interrupt
interrupts(); // enable all interrupts
}

ISR(Timer1_OVF_vect) // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
TCNT1 = 34286; // preload timer
digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
}

void loop()
{
// your program here…
}

 FOR TIME OVERFLOW

#define ledPin 13

void setup()
{
  pinMode(ledPin, OUTPUT);

  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  TCNT1 = 34286;            // preload timer
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
}

void loop()
{
  // your program here...
}


WITH COMPARE MODE 
#define ledPin 13

void setup()
{
  pinMode(ledPin, OUTPUT);
  
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 31250;            // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);   // toggle LED pin
}

void loop()
{
  // your program here...
}*/

