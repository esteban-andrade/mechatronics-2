#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);
int mode = 0;
int state =0;
volatile unsigned int  seconds;
volatile unsigned int minutes;

#define btnUP 0
#define btnDOWN 1
#define btnRIGHT 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

void setup() {
lcd.begin(16,2);

 ADMUX |=(1<<REFS0);  
 ADMUX &=~(1<<REFS1);  
 ADMUX &=~(1<<ADLAR);
ADCSRA |=(1<<ADEN)|  (1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); 

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
  sei();          // enable global interrupts
  
}
 void update_clock(){
 seconds++;
 if (seconds==60){
  seconds=0;
  minutes++;
 }
  
 }


 
int read_lcd_buttons(){

ADMUX  &=~(1<<MUX0);    //<----Set Multiplexor Value to pin A0
 ADMUX  &=~(1<<MUX1);  
 ADMUX  &=~(1<<MUX2);  
 ADMUX  &=~(1<<MUX3);        
 ADCSRA |=(1<<ADSC);      //<---- Start ADC conversion bit
  while(ADCSRA!=ADCSRA &~(1<<ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
    ADCSRA &=~(1<<ADIF);  

    delay(200);
    return ADC;
  
}



void loop(){


int x;
x= read_lcd_buttons();
 state=mode;
  if (mode==6){
    mode=0;}
if (x<50){    //right button 
  
}
if (x<195){       // up button
  
}
if (x<380){     // down button
  
}
if (x<555){       // left button
  
}
if(x<800){ 
  mode ++;
  
}
if (x>1000){
  


 
}
 
 
 // lcd.clear();
  switch (mode)
  {
    case 0:
    lcd.setCursor(0,0);
lcd.print("CLOCK");
lcd.setCursor(1,1);
lcd.print (minutes);
lcd.setCursor(3,1);
lcd.print(":");
lcd.setCursor(5,1);
lcd.print(seconds);
    break;

    case 1:
     lcd.clear();
    lcd.home();
    lcd.setCursor (0,0);
    lcd.print("ESTEBAN_A");
    lcd.setCursor(0,1);
    lcd.print("12824583");
    break;

    
    case 2:

    lcd.clear();
    lcd.home();
    lcd.print("DISTANSE SENSOR");
    break;


    case 3:
     lcd.clear();
    lcd.home();
    lcd.print("CONTINUOUS");
    break;

    case 4:
    lcd.clear();
    lcd.home();
    lcd.print("STEP");
    break;


    case 5:
    lcd.clear();
    lcd.home();
    lcd.print("LINK");
    break;


     default :
     break;
     
  }

 
    
  
 }

 
ISR(TIMER1_COMPA_vect){
  update_clock();
  int x= read_lcd_buttons();
  int *ptr=x;
  if (ADC<680){
    mode++;
  }
 if (mode==5){
  mode=0;
 }
}


