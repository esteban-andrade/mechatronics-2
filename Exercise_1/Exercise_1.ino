/*
  Exercise 1: This exercise is to turn on an LED on using the digital pin 3. 
  There will be an LED connected to DP2, DP3, and DP4. Only DP3 LED should be on.
  For this exercise you cannot use the Arduino standard library. 
  Code written by:
          Member 1 SID xxxxxxxx
          Member 2 SID xxxxxxxx
          Member 3 SID xxxxxxxx
  Date: xx/xx/xxxx
*/

#include <avr/io.h> /*Include the standard library required for programming the avr microprocessor*/

/* 
  You want to set the digital pin 3 to high. This would require the use of the I/O ports on the 
  microprocessor. Refer to the Arduino UNO schematics on the lecture slides Locate the digital   
  pin on the schematics and follow the line to figure out which pin to set high. 
  Refer to the data sheet for I/O Ports section for register addresses 
  Possible Ports
  PORTB pins 0-7
  PORTC pins 0-7
  PORTD pins 0-7
*/
  

void timer (){
TCCR0A |= (1<<WGM01);

OCR0A = 0xF9; // WAITING VALUE 


TCCR0B |= (1<<CS02);// SETS PRESCALAR

while ((TIFR0 &(1<<TOV0))>0){
  //WAIT FOR OVERFLOW
}
TIFR0|=(1<<TOV0);// RESETS FLAG



}

  

void setup() {
 DDRD |= (1<<DDD3);
 DDRD |=(1<<DDD2);
 DDRD |=(1<<DDD4);// ALL THESE sets the port to outputs

                  //for the directions   1 is output and 0 is input

                  
//PORTD |= (1<<PORTD3);             // goes to high
//PORTD &=~(1<<PORTD2);              //goes to low 
//PORTD &=~(1<<PORTD4);





    /*<-------- Here, set the Data direction register for specifc Portx Pins to output. */


}



void loop() {

   
   /*<------- Here, set the specific Portx Pinx to high and set the other two pins to low.*/
while(1){ 
PORTD |= (1<<PORTD3);   
timer();
PORTD  &=~ (1<<PORTD3);   
timer();}
}
