#include <Pixy2.h>
#include <SPI.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
Pixy2 pixy;

double sensor;
int lcd_key;


void setup() {
  lcd.begin(16, 2);              // start the library
  ADMUX |= (1 << REFS0);
  ADMUX &= ~(1 << REFS1);
  ADMUX &= ~(1 << ADLAR);
  ADCSRA |= (1 << ADEN) |  (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRA |= (1 << ADIE);

  ADCSRA |= (1 << ADSC);




  pixy.init();

}

void loop() {
  ADCSRA |= (1 << ADSC);   //<---- Start ADC conversion bit
  while (ADCSRA != ADCSRA & ~(1 << ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
    ADCSRA &= ~(1 << ADIF);



  static int i;
  int j;
  char buf[64];
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {
    i++;
    if (i % 60 == 0)

      for (j = 0; j < pixy.ccc.numBlocks ; j++) {
        if (pixy.ccc.blocks[j].m_signature == 1) {      //  set for sigbature 1      blue
          // do something for object 1
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("blue ");
        }
        if (pixy.ccc.blocks[j].m_signature == 2) {        //  set for signature 2      pink
          // do somethings for object 2
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("pink ");
        }
        if (pixy.ccc.blocks[j].m_signature == 3) {         // set for signature 3    orange
          // do something for object 3

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("orange ");
        }
        if (pixy.ccc.blocks[j].m_signature == 4) {    // set fot signature 4       yellow
          // do something  for signaure 4
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("yellow  ");
        }




      }


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

