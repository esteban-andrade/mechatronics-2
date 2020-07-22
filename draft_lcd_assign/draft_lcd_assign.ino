#include <LiquidCrystal.h>

LiquidCrystal lcd(8,9,4,5,6,7);
int mode = 0;
int state =0;
#define btnUP 0
#define btnDOWN 1
#define btnRIGHT 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

void setup() {
lcd.begin(16,2);
lcd.print("CLOCK");
lcd.setCursor(0,1);
 ADMUX |=(1<<REFS0);  
 ADMUX &=~(1<<REFS1);  
 ADMUX &=~(1<<ADLAR);
ADCSRA |=(1<<ADEN)|  (1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); 
  
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
if (x<80){    //right button 
  
}
if (x<200){       // up button
  
}
if (x<400){     // down button
  
}
if (x<600){       // left button
  
}
if(x<800){ 
  //select button
  mode ++;
}
 if (mode != state){
  switch (abs(mode))
  {
    case 0:
    lcd.clear();
    lcd.home();
    lcd.print("CLOCK");
    lcd.setCursor(0,1);
    break;

    case 1:
     lcd.clear();
    lcd.home();
    lcd.print("STUDENT ID");
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
     lcd.clear();
     lcd.home();
     lcd.print("CLOCK");
     lcd.setCursor(0,1);
     
  }

  state=mode;
  if (mode==6){
    mode=0;
  }
 }
}
 



