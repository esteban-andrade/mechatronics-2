//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <avr/io.h>       //Include AVR library
/*******************************************************
Modify this program's check buttons to operate with the AVR ADC instead of the arduino library.
Original by: Mark Bramwell, July 2010 Modified by: Howe Zhu 
********************************************************/
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
// define some values used by the panel and buttons

#define btnUP 0
#define btnDOWN 1
#define btnRIGHT 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

double sensor, inches, cm ;
int lcd_key;
volatile unsigned int  seconds;
volatile unsigned int minutes;
int mode=0;


void setup()
{
/* Set up ADC between here */
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

int distance(){
   ADCSRA |=(1<<ADSC);      //<---- Start ADC conversion bit
  while(ADCSRA!=ADCSRA &~(1<<ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
  ADCSRA &=~(1<<ADIF);
  inches = 4192.936 * pow(sensor, -0.935) - 3.937;
  cm = 10650.08 * pow(sensor, -0.935) - 10;
  delay(300);

  return sensor;
}

void loop()
{

   if (ADC < 555){
      lcd.setCursor(0,0);
       lcd.print ("lol");
      
    }
lcd.setCursor(0,1);            // move to the begining of the second line
lcd_key = read_LCD_buttons();  // Calls read button function

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.print("RIGHT ");
     break;
     }
   case btnLEFT:
     {
     lcd.print("LEFT   ");
     break;
     }
   case btnUP:
     {
     lcd.print("UP    ");
     break;
     }
   case btnDOWN:
     {
     lcd.print("DOWN  ");
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT");
     break;
     }
     case btnNONE:
     {
     lcd.print("NONE  ");
     break;
     }
 }
}

int read_LCD_buttons()
{
 ADMUX  &=~(1<<MUX0);    //<----Set Multiplexor Value to pin A0
 ADMUX  &=~(1<<MUX1);  
 ADMUX  &=~(1<<MUX2);  
 ADMUX  &=~(1<<MUX3);        
 ADCSRA |=(1<<ADSC);      //<---- Start ADC conversion bit
  while(ADCSRA!=ADCSRA &~(1<<ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
  ADCSRA &=~(1<<ADIF);     //<---- Clear interrupt flag by setting bit to 1
  if (ADC > 1000) return btnNONE; // We make this the 1st option for speed reasons since itwill be the most likely result
  if (ADC < 50) return btnRIGHT;   //These are your 10bit value thresholds 0-1023
  if (ADC < 195) return btnUP;
  if (ADC < 380) return btnDOWN;
  if (ADC < 555) return btnLEFT;
  if (ADC < 790){
  lcd.print("SELECT");
          if ( (ADC < 790)){
            lcd.setCursor(5,1);
            lcd.print(":)");
          } return btnSELECT;
  return btnSELECT; }
  return btnNONE ; // when all others fail, return this...
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
