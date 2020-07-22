#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
int LCD_mode = 0;
int lcd_key=0;
int adc_key_in  = 0;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 0.1;    // the debounce time
volatile unsigned int  seconds;
volatile unsigned int minutes;


void setup() {
  // put your setup code here, to run once:
 lcd.begin(16, 2);              // start the library
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
  lcd.clear();
 }
  
 }

ISR(TIMER1_COMPA_vect){
  update_clock();
 
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

void toggleMode(){
  adc_key_in = read_lcd_buttons();
  if(adc_key_in < 800){ //select key ADC value
    if ((seconds - lastDebounceTime) > debounceDelay) { //d
      if(LCD_mode < 5){
        lcd.clear();
        LCD_mode++;
      }
      else{
        lcd.clear();
        LCD_mode = 0;
      }
      lastDebounceTime = seconds;
    }
}
}

void loop() {
  toggleMode();
  lcd.setCursor(0,0);

  switch(LCD_mode){
  case 0:
    {
     
   
lcd.setCursor(0,0);
lcd.print("CLOCK");
lcd.setCursor(1,1);
lcd.print (minutes);
lcd.setCursor(3,1);
lcd.print(":");
lcd.setCursor(5,1);
lcd.print(seconds);
    break;
    }

  case 1:
    {
     lcd.setCursor (0,0);
    lcd.print("ESTEBAN_A");
    lcd.setCursor(0,1);
    lcd.print("12824583");
    break;
    }
    
  case 2:
    {
    lcd.print("DISTANCE SENSOR");
    lcd.setCursor(8,1);
    lcd.print("CM");
    break;
    }

  case 3:
    {
    lcd.print("CONTINUOUS");
    break;
    }

  case 4:
    { 
    lcd.print("STEP");
    break;
    }

  case 5:
    {
    lcd.print("LINK");
    break;
    }
 }
}


