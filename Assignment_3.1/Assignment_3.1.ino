
#include <Servo.h>
#include <LiquidCrystal.h>

Servo servo;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#include <Pixy2.h>
#include <PIDLoop.h>
#include <SPI.h>
Pixy2 pixy;               //needed to innitialise
PIDLoop panLoop(400, 0, 400, true);       //sets the PID to ensure that we get accurate results
PIDLoop tiltLoop(500, 0, 500, true);

#define btnUP 0
#define btnDOWN 1
#define btnRIGHT 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

int servoIdle = 0;     //Servo starting position
int servoPin = 3;     // pin the servo is connected to
int sensor;
int lcd_key;         
int previous_button = btnNONE;
int current_button = btnNONE;
int btn_count = 0;
int mode;




void setup() 
{
 
  servoSetup();
  lcd.begin(16, 2);     // start the lcd
  Serial.begin(115200);
  pixy.init();      // start camera
  //lcd.setCursor(0, 0); // sets the cursor to the first position of the second line
  //lcd.print("Colour Detection"); 
  pixy.setLamp(0,0); 


// this is done to start the adc conversion 

 ADMUX |= (1 << REFS0);
  ADMUX &= ~(1 << REFS1);
  ADMUX &= ~(1 << ADLAR);
  ADCSRA |= (1 << ADEN) |  (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRA |= (1 << ADIE);

}

void servoSetup()
{
  servo.attach(servoPin);
  servo.write(servoIdle);
}
int read_LCD_buttons()
{
  if (lcd_key > 1000)  return btnNONE; 
  if (lcd_key < 80)    return btnRIGHT;
  if (lcd_key < 195)   return btnUP;
  if (lcd_key < 380)   return btnDOWN;
  if (lcd_key < 555)   return btnLEFT;
  if (lcd_key < 790)  {
   mode++;
    return btnSELECT; 
  }

  
  return btnNONE; // when all others fail, return this
  
}

int debounce_buttons()
{
  current_button = read_LCD_buttons();
  if (current_button == previous_button)
  {
    return btnNONE;
  }
  if (current_button != previous_button)
  {
    if (btn_count < 50)
    {
      current_button = btnNONE;
      btn_count++;
    }
    else
    {
      previous_button = current_button;
      btn_count = 0;   
    }
    return current_button;
  }
}


void loop() 




{

  read_LCD_buttons();


  pixy.ccc.getBlocks();

  ADCSRA |= (1 << ADSC);   //<---- Start ADC conversion bit
  while (ADCSRA != ADCSRA & ~(1 << ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
    ADCSRA &= ~(1 << ADIF);



    switch(mode){
     case 0:
     
      lcd.setCursor(0,0);
        lcd.print("PRESS SELECT");
        lcd.setCursor(0,1);
        lcd.print(" TO START");
     
      break;

      case 1:

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("SCANNING");
  if (pixy.ccc.numBlocks)
  {       
    static int i = 0;
    int j;
    char buf[64]; 
    int32_t panOffset, tiltOffset; 
    i++;
    
    if (i%60==0)          //if an object is detected..
      //erial.println(i);   
      
    panOffset = (int32_t)pixy.frameWidth/2 - (int32_t)pixy.ccc.blocks[0].m_x;   //calculate the objects offset from the centre of the screen in the x direction
    tiltOffset = (int32_t)pixy.ccc.blocks[0].m_y - (int32_t)pixy.frameHeight/2;  //calculate the objects offset from the centre of the screen in the y direction

    if (panOffset > 0)
    {
    lcd.setCursor(0, 0);
    lcd.print("move left");
    lcd.print("    ");
    }
    else if (panOffset < 0)
    {
    lcd.setCursor(0, 0);
    lcd.print("move right");
    lcd.print("    ");
    }

    if (tiltOffset > 0)
    {
    lcd.setCursor(0, 1);
    lcd.print("move backwards");
    lcd.print("    ");
    }
    else if (tiltOffset < 0)
    {
    lcd.setCursor(0, 1);
    lcd.print("move forwards");
    lcd.print("    ");
    }

    
  
    panLoop.update(panOffset);
    tiltLoop.update(tiltOffset);
    
    pixy.setServos(500, 100);     //sets the angle of the servos

   
   //lcd.setCursor(0, 1);
   //lcd.print("green detected");

#if 0
    sprintf(buf, "%ld %ld %ld %ld", rotateLoop.m_command, translateLoop.m_command, left, right);
    Serial.println(buf);   
#endif
  }  
  else // no object detected, go into reset state
  {
    panLoop.reset();
    tiltLoop.reset();
    pixy.setServos(500, 100);
    lcd.setCursor(0, 1);
    //lcd.print("nothing detected");
  }
      
      break;
    }

  




      

}










ISR(ADC_vect) {
  uint16_t result = ADC;
  switch (ADMUX & 0x0F) { //Here we are only looking at the last for bits by setting the first for bits to 0 (just for the switch statement the bits in ADMUX don't actually change)

    case 0: //Don't need the C0 --> only 0 as we cleared the first 4 bits ^^
      lcd_key = result;                   //retrieves the value of the adc that will go into the button for all the funtions 
      ADMUX |= (1 << MUX0); //just change the last bit to change the channel

      break;


    case 1 :


      sensor = result;                                //retrieve the value of the adc module of channel 1 that will be used for the sensor 
      ADMUX &= ~(1 << MUX0);
      break;

    default:


      break;
  }


  ADCSRA |= (1 << ADSC);

}

