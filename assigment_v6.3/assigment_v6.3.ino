#include <avr/io.h>       //Include AVR library

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

double sensor, inches, cm ;
int lcd_key =0;
volatile unsigned int  seconds;
volatile unsigned int minutes;
int mode =0 ;
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
  sei();          // enable global interrupts

  ADCSRA |= (1 << ADSC);
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
    
   ADCSRA |=(1<<ADSC);      //<---- Start ADC conversion bit
  while(ADCSRA!=ADCSRA &~(1<<ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
  ADCSRA &=~(1<<ADIF);

  if (  lcd_key < 850) {
  mode++;
  lcd.clear();

}
 // return btnSELECT; 
  
  switch (mode){
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
      lcd.setCursor (0, 0);
        lcd.print("ESTEBAN_A");
        lcd.setCursor(0, 1);
        lcd.print("12824583");
    break;

    
    case 2:

         lcd.print("DISTANCE SENSOR");
        lcd.setCursor(8, 1);
        lcd.print("CM");
        lcd.setCursor(0, 1);
        lcd.print(cm);
    break;


    case 3:
    lcd.print("CONTINUOUS");
    break;

    case 4:
       lcd.print("STEP");
    break;


    case 5:
    ;
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
  delay(100);

  return sensor;
}
 



void loop() {
  // put your main code here, to run repeatedly:
 read_LCD_buttons();
    distance();
 ADCSRA |= (1 << ADSC);   //<---- Start ADC conversion bit
  while (ADCSRA != ADCSRA & ~(1 << ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
    ADCSRA &= ~(1 << ADIF);


    
}


ISR(TIMER1_COMPA_vect) {
  update_clock();

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


