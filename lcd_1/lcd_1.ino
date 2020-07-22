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
long debounceDelay = 500;    // the debounce time

void toggleMode(){
  adc_key_in = analogRead(0);
  if(adc_key_in < 650){ //select key ADC value
    if ((millis() - lastDebounceTime) > debounceDelay) { //d
      if(LCD_mode < 5){
        lcd.clear();
        LCD_mode++;
      }
      else{
        lcd.clear();
        LCD_mode = 0;
      }
      lastDebounceTime = millis();
    }
}
}

void setup() {
  // put your setup code here, to run once:
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
}

void loop() {
  toggleMode();
  lcd.setCursor(0,0);

  switch(LCD_mode){
  case 0:
    {
    lcd.print("CLOCK MODE");
    break;
    }

  case 1:
    {
    lcd.print("STUDENT ID");
    break;
    }
    
  case 2:
    {
    lcd.print("DISTANCE");
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


