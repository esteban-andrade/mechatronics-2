#include <avr/io.h>       //Include AVR library

#include <Time.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

double sensor, inches, cm ;
int lcd_key;
volatile unsigned int  seconds;
volatile unsigned int minutes;
int mode=1;



int Steps = 0;      // Start at no steps in sequence
int Direction; //Defaults at clockwise
int stepsleft; //<----Calculate the total number of steps per revolution
int x;
float speeder =5.0 ;
int count;


void setup() {
  // put your setup code here, to run once:
 lcd.begin(16, 2);              // start the library
  ADMUX |= (1 << REFS0);
  ADMUX &= ~(1 << REFS1);
  ADMUX &= ~(1 << ADLAR);
  ADCSRA |= (1 << ADEN) |  (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRA |= (1 << ADIE);

  //timer 1 setup
 cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
  OCR1A = 15624;
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);



 TCCR2A &=~(1<<  COM2A1);
  
        TCCR2A &=~(1<<COM2A0);//<-------- Here, set control register A for the normal mode of operation.
        TCCR2B |=(1<<CS22)|(1<<CS21)|(1<<CS20);//<-------- Here, set control register B for the prescaler values choose wisely to get 1ms.
        TCNT2=0; //Initial counter at 0   

  
  sei();      // enable global interrupts

  ADCSRA |= (1 << ADSC);


  DDRB |=(1<DDB5)|(1<<DDB4)|(1<<DDB3);  //<-----Choose which pins to set as output for the motor, avoid using the digital pins the LCD uses.
  DDRD |=(1<<DDD3);
 
 Serial.begin(9600);
}
void update_clock() {
  seconds++;
  if (seconds == 60) {
    seconds = 0;
    minutes++;
    lcd.clear();
  }

}






int  read_LCD_buttons(){

if (lcd_key <750 && lcd_key>600){

delayer();
//delay(200);
//mode++;
lcd.clear();  
Serial.println("l");
   }
   

if ( mode==7){
  mode=1;
}

if (mode==3){
  
}
  
   ADCSRA |=(1<<ADSC);      //<---- Start ADC conversion bit
  while(ADCSRA!=ADCSRA &~(1<<ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
  ADCSRA &=~(1<<ADIF);

 
  switch (mode){
  case 1:
   lcd.setCursor(0,0);
lcd.print("CLOCK");
lcd.setCursor(1,1);
lcd.print (minutes);
lcd.setCursor(3,1);
lcd.print(":");
lcd.setCursor(5,1);
lcd.print(seconds);
    break;

    case 2:
      lcd.setCursor (0, 0);
        lcd.print("ESTEBAN A");
        lcd.setCursor(0, 1);
        lcd.print("12824583");
    break;

    
    case 3:
  lcd.setCursor(0,0);
         lcd.print("DISTANCE SENSOR");
        lcd.setCursor(8, 1);
        lcd.print("CM");
        lcd.setCursor(0, 1);
        lcd.print(cm);
         //distance();
        
    break;


    case 4:
    lcd.setCursor(0,0);
    lcd.print("CONTINUOUS");
   conti();
    //continous();
    
    break;

    case 5:
    lcd.setCursor(0,0);
       lcd.print("STEPS");
       lcd.setCursor(0,1);
       lcd.print(stepsleft);
       steps();
    break;


    case 6:
    lcd.setCursor(0,0);;
    lcd.print("LINK");
    break;
    
  }

  return  lcd_key;
}


int distance(){
   ADCSRA |=(1<<ADSC);      //<---- Start ADC conversion bit
  while(ADCSRA!=ADCSRA &~(1<<ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
  ADCSRA &=~(1<<ADIF);
  inches = 4192.936 * pow(sensor, -0.935) - 3.937;
  cm = 10650.08 * pow(sensor, -0.935) - 10;
  //delay(200);
//delayer();
  return sensor;
}
 
void SetDirection(){

  
      if(Direction==x)
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
void motor(int xw){
    for (int x=0;x<xw;x++)
  {
   

     switch(Steps)     // This switch case will handle the digital pins to toggle for each step, refer to the step sequence table for pin values. 
      {                 // Make sure you are toggling the correct pins
       case 0:
                      //<----- Insert Digital Pin allocations for steps 1-7 Remember do not set the whole byte because the LCD uses certain digital pins.
        PORTB |=(1<<PORTB5);  
        PORTD &=~(1<<PORTD3);
        PORTB &=~(1<<PORTB3);
        PORTB &=~(1<<PORTB4);
         //delay(1);
         
         break; 
     case 1:
     PORTB  |=(1<<PORTB5);
     PORTB  |=(1<<PORTB4);   
     PORTD &=~(1<<PORTD3);
     PORTB &=~(1<<PORTB3);
    //delay(1);
         break; 
         case 2:
    PORTB |=(1<<PORTB4); 
    PORTD &=~(1<<PORTD3);
    PORTB &=~(1<<PORTB3);  
     PORTB &=~(1<<PORTB5); 
       //delay(1);
         break; 
         case 3:
     PORTB |=(1<<PORTB4);
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

void stepper(){
 
   

      switch(Steps)     // This switch case will handle the digital pins to toggle for each step, refer to the step sequence table for pin values. 
      {                 // Make sure you are toggling the correct pins
       case 0:
                      //<----- Insert Digital Pin allocations for steps 1-7 Remember do not set the whole byte because the LCD uses certain digital pins.
        PORTB |=(1<<PORTB5);  
        PORTD &=~(1<<PORTD3);
        PORTB &=~(1<<PORTB3);
        PORTB &=~(1<<PORTB4);
         //delay(1);
         
         break; 
     case 1:
     PORTB  |=(1<<PORTB5);
     PORTB  |=(1<<PORTB4);   
     PORTD &=~(1<<PORTD3);
     PORTB &=~(1<<PORTB3);
    //delay(1);
         break; 
         case 2:
    PORTB |=(1<<PORTB4); 
    PORTD &=~(1<<PORTD3);
    PORTB &=~(1<<PORTB3);  
     PORTB &=~(1<<PORTB5); 
       //delay(1);
         break; 
         case 3:
     PORTB |=(1<<PORTB4);
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

void conti(){
 
SetDirection();
stepper ();
delay(speeder);

if (lcd_key<555  && lcd_key>350){ // btnleft
  x=1;
  
}
if (lcd_key<50 ){
  x=0;
              //btn right
  
}
 if (lcd_key<175 && lcd_key>50){  //btnup
   speeder=speeder-0.1;
  
} 

if (lcd_key<340 && lcd_key>200){
   speeder=speeder+0.1;
   
}   // btn down
if (speeder<=0.1){
  speeder=0.1;
}
}


void steps(){
  int rot= abs(stepsleft);


  
  
   while (lcd_key<555  && lcd_key>350){ // btnleft
  stepsleft++;
  delay(2);
  return stepsleft;
  
}
while (lcd_key<50 ){
  stepsleft--;
  delay(2);
  return stepsleft;
              //btn right
  
}
if (lcd_key<175 && lcd_key>50){
  lcd.clear();                        //btnUP
  stepsleft=0;
}return stepsleft;

if (lcd_key<340 && lcd_key>200){


while (stepsleft>0){

 SetDirection();
motor(rot);
}
}return stepsleft;

SetDirection();
motor(1);


}




/*int continous(){
  if ( lcd_key<555  && lcd_key>400){
  x=1;
  while(stepsleft>0){





      if (TCNT0>=250)    //<--- Decide Timer counter value to step at, goal is 1ms
    {
      stepper();         //<--- Step a certain amount
      stepsleft--;        //<--- Decrease Step count
      TCNT0=0;
    }
  
  //Direction=!Direction; // Reverse Direction
 //stepsleft=4096;     //<---- Reset to initial value 
}
 }
  if (lcd_key<50 ){
  x=0;
  while(stepsleft>0){





      if (TCNT0>=250)    //<--- Decide Timer counter value to step at, goal is 1ms
    {
      stepper();         //<--- Step a certain amount
      stepsleft--;        //<--- Decrease Step count
      TCNT0=0;
    }
  
  // // Reverse Direction
 //stepsleft=4096;     //<---- Reset to initial value 
}
 



  return x;
}

}
*/

void delayer(){

 if ( TCNT2>=16){
  count++;
   TCNT2=0;

 } if (count==40){
   mode++;
  count =0;
 
 }
}

void delaysensor(){
  if ( TCNT2>=16){
  count++;
   TCNT2=0;

 } if (count==20){
   distance();
  count =0;
 
 }
}


  

void loop() {
  // put your main code here, to run repeatedly:


 read_LCD_buttons();
  
 
//if (mode==4){
  //continous();
//}




    
 ADCSRA |= (1 << ADSC);   //<---- Start ADC conversion bit
  while (ADCSRA != ADCSRA & ~(1 << ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
    ADCSRA &= ~(1 << ADIF);


 
   
}


ISR(TIMER1_COMPA_vect) {
  update_clock();
//count++;
}


ISR(ADC_vect) {
  uint16_t result = ADC;
  switch (ADMUX & 0x0F) { //Here we are only looking at the last for bits by setting the first for bits to 0 (just for the switch statement the bits in ADMUX don't actually change)

    case 0: //Don't need the C0 --> only 0 as we cleared the first 4 bits ^^
      lcd_key =result;
      ADMUX |= (1 << MUX0); //just change the last bit to change the channel
   
      break;


    case 1 :

     
      sensor=result;
      ADMUX &= ~(1 << MUX0);
      break;

    default:
    

      break;
  }


  ADCSRA |= (1 << ADSC);

}


