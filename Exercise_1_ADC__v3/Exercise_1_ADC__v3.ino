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
int lcd_key = 0;
#define btnUP 0
#define btnDOWN 1
#define btnRIGHT 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5



void setup()
{
/* Set up ADC between here */
 ADMUX |=(1<<REFS0);            //<---- Setup ADC Multiplexer Selection Register, so that Vcc is the Ref, it should be "AVCC with external capacitor at AREF pin"
 ADMUX &=~(1<<REFS1);  
 ADMUX &=~(1<<ADLAR);            //<---- Setup ADC Multiplexer Selection Register, so that the result is right justified
ADCSRA |=(1<<ADEN)|  (1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);           //<---- Setup ADC Control and Status Register A, so that ADC is enabled, set up conversion rate to 125kHz. Remember Clock/Prescaler, you can select prescaler
/* Set up ADC between here */

lcd.begin(16, 2); // start the library
lcd.setCursor(0,0);
lcd.print("Push the buttons"); // print a simple message
}



void loop()
{
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
  if (ADC < 790) return btnSELECT; 
  return btnNONE; // when all others fail, return this...
}
