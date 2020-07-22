/*
  Exercise 2: This exercise is for the stepper motor to cycle back and forth between clockwise and counter clockwise rotations. Do not use standard Functions
  Code written by:
          Member 1 SID xxxxxxxx
          Member 2 SID xxxxxxxx
          Member 3 SID xxxxxxxx
  Date: xx/xx/xxxx
*/

#include <avr/io.h>

int Steps =4096;      // Start at no steps in sequence
int Direction = 0; //Defaults at clockwise
int stepsleft=4096; //<----Calculate the total number of steps per revolution

void setup() {
      DDRB |=(1<DDB5)|(1<<DDB4)|(1<<DDB3);  //<-----Choose which pins to set as output for the motor, avoid using the digital pins the LCD uses.
  DDRD |=(1<<DDD3);//<-----Choose which pins to set as output for the motor, avoid using the digital pins the LCD uses.
        TCCR0A &=~(1<<  COM0A1);
        TCCR0A &=~(1<<COM0A0);//<-------- Here, set control register A for the normal mode of operation.
        TCCR0B |=(1<<CS01)|(1<<CS00);//<-------- Here, set control register B for the prescaler values choose wisely to get 1ms.
        TCNT0=0; //Initial counter at 0         
}




void loop() {
 while(stepsleft>0){
      if (TCNT0>=250)    //<--- Decide Timer counter value to step at, goal is 1ms
    {
      stepper(1);         //<--- Step a certain amount
      stepsleft--;        //<--- Decrease Step count
      TCNT0=0;
    }
  }
  Direction=!Direction; // Reverse Direction
  stepsleft=4096;     //<---- Reset to initial value 
}

/* This function handles the digital pin toggles to drive the motor */
void stepper(int xw){
  for (int x=0;x<xw;x++)
  {
   

      switch(Steps)     // This switch case will handle the digital pins to toggle for each step, refer to the step sequence table for pin values. 
      {                 // Make sure you are toggling the correct pins
       
     PORTB |=(1<<PORTB3); 
     PORTD &=~(1<<PORTD3); 
     PORTB &=~(1<<PORTB5);  
       //delay(1);
         break; 
         case 4:
        PORTB |=(1<<PORTB3);  
        PORTD &=~(1<<PORTD3); 
        PORTB &=~(1<<PORTB4);
        PORTB &=~(1<<PORTB5);
     //delay(1);
         break; 
         case 5:
      PORTB |=(1<<PORTB3);
      PORTD |=(1<<PORTD3); 
      PORTB &=~(1<<PORTB4);
      PORTB &=~(1<<PORTB5); 
          // delay(1);   
         break; 
           case 6:
      PORTD |=(1<<PORTD3);  
      PORTB &=~(1<<PORTB3);
      PORTB &=~(1<<PORTB4);
      PORTB &=~(1<<PORTB5);
        //delay(1);
         break; 
         case 7:
      PORTD |=(1<<PORTD3);  
      PORTB &=~(1<<PORTB3);
      PORTB &=~(1<<PORTB4);
      PORTB |=(1<<PORTB5); 
         //delay(1);   
         break; 
         default:
    
      
       PORTD &=~(1<<PORTD3);
       PORTB &=~(1<<PORTB3);
       PORTB &=~(1<<PORTB4);
       PORTB &=~(1<<PORTB5);//<----Default pins select are off
         break;  
      }
    SetDirection();
  }
} 

/* This Function handles the resetting of direction, when reaching the end of the rotation */
void SetDirection(){
      if(Direction==1)
      { 
        Steps++;
      }
      else
      { 
        Steps--; 
      }
      if(Steps>7){
        Steps=0;
      }
      if(Steps<0)
      {
        Steps=7; 
      }
}

