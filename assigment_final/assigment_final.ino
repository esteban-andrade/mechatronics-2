#include <avr/io.h>       //Include AVR library
#include<stdio.h>

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

double sensor, inches, cm,dist1, dist2;
int lcd_key;
volatile unsigned int  seconds;
volatile unsigned int minutes;
int mode = 1;
int angle;
double avg;
double reads;
int Steps = 0;      // Start at no steps in sequence
int Direction; //Defaults at clockwise
int stepsleft; //<----Calculate the total number of steps per revolution
int x;          // variable to hold the direction
int speeder ;           //variable that set the speed of the motor
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

  //Serial.begin(9600);

  TCCR2A &= ~(1 <<  COM2A1);

  TCCR2A &= ~(1 << COM2A0); //<-------- Here, set control register A for the normal mode of operation.
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); //<-------- Here, set control register B for the prescaler values choose wisely to get 1ms.
  TCNT2 = 0; //Initial counter at 0


  sei();      // enable global interrupts

  ADCSRA |= (1 << ADSC);              //runs tha adc module


  DDRB |= (1 < DDB5) | (1 << DDB4) | (1 << DDB3); //<-----Choose which pins to set as output for the motor, avoid using the digital pins the LCD uses.
  DDRD |= (1 << DDD3);


}
void update_clock() {                   //function that counts for the clock mode
  seconds++;                            // count the count of seconds it will increment
  if (seconds == 60) {                    //checks the conditions to then increase the minutes
    seconds = 0;
    minutes++;
    lcd.clear();
  }

}



int  read_LCD_buttons() {


  if (lcd_key < 750 && lcd_key > 600) {           //threshold value of the select button
    if ( TCNT2 >= 16) {
      count++;
      TCNT2 = 0;

    } if (count == 40) {                  //debouncer for the button as this will wait a  amount of time until it changes to the next mode
      mode++;
      count = 0;
      lcd.clear();
    }

  }


  if ( mode == 7) {                         // checks is the modes goes to the final moden and hence goes back to the first mode and keeps the cycle
    mode = 1;
  }


  ADCSRA |= (1 << ADSC);   //<---- Start ADC conversion bit
  while (ADCSRA != ADCSRA & ~(1 << ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
    ADCSRA &= ~(1 << ADIF);


  switch (mode) {
    case 1:

      lcd.home();
      lcd.setCursor(0, 0);
      lcd.print("CLOCK");
      lcd.setCursor(1, 1);
      lcd.print (minutes);
      lcd.setCursor(3, 1);
      lcd.print(":");
      lcd.setCursor(5, 1);
      lcd.print(seconds);
      break;

    case 2:
      lcd.setCursor (0, 0);
      lcd.print("ESTEBAN A");
      lcd.setCursor(0, 1);
      lcd.print("12824583");
      break;


    case 3:
      lcd.setCursor(0, 0);
      lcd.print("DISTANCE SENSOR");
      lcd.setCursor(8, 1);
      lcd.print("cm");
      lcd.setCursor(0, 1);
      lcd.print(avg);
      distance();
      break;


    case 4:
      lcd.setCursor(0, 0);
      lcd.print("CONTINUOUS");

      conti();

      break;

    case 5:
      lcd.setCursor(0, 0);
      lcd.print("STEPS");
      lcd.setCursor(0, 1);
      lcd.print(stepsleft);
      steps();
      break;


    case 6:
      lcd.setCursor(0, 0);;
      lcd.print("LINK");
      lcd.setCursor(1, 1);
      lcd.print(angle);
      linki();
      break;

  }

  return  lcd_key;
}


int distance() {
  if (lcd_key < 750 && lcd_key > 600) {
    if ( TCNT2 >= 16) {
      count++;
      TCNT2 = 0;

    } if (count == 20) {
      mode++;
      count = 0;
      lcd.clear();                          //this was done with the purpose to exit the distance mode when the select is pressed.
    }



  }

  float total;        // sets a float variable
  ADCSRA |= (1 << ADSC);   //<---- Start ADC conversion bit
  while (ADCSRA != ADCSRA & ~(1 << ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
    ADCSRA &= ~(1 << ADIF);

  for (int x = 0; x < 200; x++) {                   // seta a for loop to average and poll the value that the sensor is reading
    inches = 4192.936 * pow(sensor, -0.935) - 3.937;
    cm = 10650.08 * pow(sensor, -0.935) - 10;         // distance equations
   dist1=60.374*pow(sensor,-1.16);
   dist2=23207.9*pow(sensor,-1.1171);
    
    
    total = dist2;
  }
  if ( TCNT2 >= 16) {

    count++;
    TCNT2 = 0;

  }

  if (count == 40) {
    avg = total;
    reads = total;                    //this was done with the purpose of giving the sensor a stable update rate
    count = 0;
  }


  return sensor;            // returns the value of the adc channel 1 from isr
}

void SetDirection() {


  if (Direction == x)                 //check whether the conditino for the funtions is either 1 or  0 to set a direction 
  {
    Steps++;
  }
  else
  {
    Steps--;
  }
  if (Steps > 7) {
    Steps = 0;
  }
  if (Steps < 0)
  {
    Steps = 7;
  }
}
void motor(float xw) {
  for (float x = 0.0; x < xw; x++)
  {


    switch (Steps)    // This switch case will handle the digital pins to toggle for each step, refer to the step sequence table for pin values.
    { // Make sure you are toggling the correct pins
      case 0:
        //<----- Insert Digital Pin allocations for steps 1-7 Remember do not set the whole byte because the LCD uses certain digital pins.
        PORTB |= (1 << PORTB5);
        PORTD &= ~(1 << PORTD3);
        PORTB &= ~(1 << PORTB3);
        PORTB &= ~(1 << PORTB4);

        break;
      case 1:
        PORTB  |= (1 << PORTB5);
        PORTB  |= (1 << PORTB4);
        PORTD &= ~(1 << PORTD3);
        PORTB &= ~(1 << PORTB3);

        break;
      case 2:
        PORTB |= (1 << PORTB4);
        PORTD &= ~(1 << PORTD3);
        PORTB &= ~(1 << PORTB3);
        PORTB &= ~(1 << PORTB5);

        break;
      case 3:
        PORTB |= (1 << PORTB4);
        PORTB |= (1 << PORTB3);
        PORTD &= ~(1 << PORTD3);
        PORTB &= ~(1 << PORTB5);

        break;
      case 4:
        PORTB |= (1 << PORTB3);
        PORTD &= ~(1 << PORTD3);
        PORTB &= ~(1 << PORTB4);
        PORTB &= ~(1 << PORTB5);

        break;
      case 5:
        PORTB |= (1 << PORTB3);
        PORTD |= (1 << PORTD3);
        PORTB &= ~(1 << PORTB4);
        PORTB &= ~(1 << PORTB5);

        break;
      case 6:
        PORTD |= (1 << PORTD3);
        PORTB &= ~(1 << PORTB3);
        PORTB &= ~(1 << PORTB4);
        PORTB &= ~(1 << PORTB5);

        break;
      case 7:
        PORTD |= (1 << PORTD3);
        PORTB &= ~(1 << PORTB3);
        PORTB &= ~(1 << PORTB4);
        PORTB |= (1 << PORTB5);

        break;
      default:


        PORTD &= ~(1 << PORTD3);
        PORTB &= ~(1 << PORTB3);
        PORTB &= ~(1 << PORTB4);
        PORTB &= ~(1 << PORTB5); //<----Default pins select are off
        break;
    }
    SetDirection();
  }
}

void stepper() {



  switch (Steps)    // This switch case will handle the digital pins to toggle for each step, refer to the step sequence table for pin values.
  { // Make sure you are toggling the correct pins
    case 0:
      //<----- Insert Digital Pin allocations for steps 1-7 Remember do not set the whole byte because the LCD uses certain digital pins.
      PORTB |= (1 << PORTB5);
      PORTD &= ~(1 << PORTD3);
      PORTB &= ~(1 << PORTB3);
      PORTB &= ~(1 << PORTB4);

      break;
    case 1:
      PORTB  |= (1 << PORTB5);
      PORTB  |= (1 << PORTB4);
      PORTD &= ~(1 << PORTD3);
      PORTB &= ~(1 << PORTB3);

      break;
    case 2:
      PORTB |= (1 << PORTB4);
      PORTD &= ~(1 << PORTD3);
      PORTB &= ~(1 << PORTB3);
      PORTB &= ~(1 << PORTB5);

      break;
    case 3:
      PORTB |= (1 << PORTB4);
      PORTB |= (1 << PORTB3);
      PORTD &= ~(1 << PORTD3);
      PORTB &= ~(1 << PORTB5);

      break;
    case 4:
      PORTB |= (1 << PORTB3);
      PORTD &= ~(1 << PORTD3);
      PORTB &= ~(1 << PORTB4);
      PORTB &= ~(1 << PORTB5);
      //delay(1);
      break;
    case 5:
      PORTB |= (1 << PORTB3);
      PORTD |= (1 << PORTD3);
      PORTB &= ~(1 << PORTB4);
      PORTB &= ~(1 << PORTB5);

      break;
    case 6:
      PORTD |= (1 << PORTD3);
      PORTB &= ~(1 << PORTB3);
      PORTB &= ~(1 << PORTB4);
      PORTB &= ~(1 << PORTB5);
      //delay(1);
      break;
    case 7:
      PORTD |= (1 << PORTD3);
      PORTB &= ~(1 << PORTB3);
      PORTB &= ~(1 << PORTB4);
      PORTB |= (1 << PORTB5);

      break;
    default:


      PORTD &= ~(1 << PORTD3);
      PORTB &= ~(1 << PORTB3);
      PORTB &= ~(1 << PORTB4);
      PORTB &= ~(1 << PORTB5); //<----Default pins select are off
      break;
  }
  SetDirection();

}

void conti() {



  if ( TCNT2 >= 20 + speeder) {                       //this set the speed of the motor to a particular speed and then a variable will be added in order to increase or decrease the value of the speed based on other conditions
    SetDirection();
    stepper ();                                           // start the motor spinning 
    if (lcd_key < 750 && lcd_key > 600) {
      if ( TCNT2 >= 16) {
        count++;
        TCNT2 = 0;

      } if (count == 40) {
        mode++;
        count = 0;
        lcd.clear();                                  // this check whether the selct button is pressed and hence moves to the next mode
      }



    }

    TCNT2 = 0;

  }

  if (lcd_key < 555  && lcd_key > 350) { // btnleft
    x = 1;                                  // sets direction counterclockwise aand will get called on the direction function
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(speeder);
    lcd.setCursor(4, 1);
    lcd.print("rate");
    lcd.setCursor(9, 1);
    lcd.print("CCW");                       // prints the speed and direction 


  }
  if (lcd_key < 50 ) {      //btn right

    x = 0;                                    // sets the direction clockwise and will get called on the direcion  function 
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(speeder);
    lcd.setCursor(4, 1);
    lcd.print("rate");
    lcd.setCursor(9, 1);
    lcd.print("CW");                                //prints speed and direction
   
  }
  if (lcd_key < 175 && lcd_key > 50) { //btnup
    speeder = speeder - 1;                              // variable that increases the speed of the motor 
    lcd.setCursor(0, 1);
    lcd.print(speeder);
    lcd.setCursor(4, 1);            
    lcd.print("rate");

  }

  if (lcd_key < 340 && lcd_key > 200) {
    speeder = speeder + 1;                              //variable that increases and hence decreases the speed of the motor 
    lcd.setCursor(0, 1);
    lcd.print(speeder);
    lcd.setCursor(4, 1);
    lcd.print("rate");

  }   // btn down
  if (speeder == 0) {                                 // avoids the motor to go back stop so it set the maximum speed
    speeder = 1;
  }



  if (lcd_key < 750 && lcd_key > 600) {
    if ( TCNT2 >= 16) {
      count++;
      TCNT2 = 0;

    } if (count == 40) {                              //check condition if the select button gets pressed and moves to the next mode
      mode++;
      count = 0;
      lcd.clear();
    }


  }
}


void steps() {


  if (lcd_key < 555  && lcd_key > 350) {            //btn left 
    if ( TCNT2 >= 16) {
      count++;                                    //debounces the button 
      TCNT2 = 0;

    } if (count == 40) {
      stepsleft = stepsleft + 100;                    //increases the amount of steps it will rotate 
      count = 0;
      if (stepsleft == 4097) {
        lcd.clear();
        stepsleft = 0;

      }
    }


  }


  if (lcd_key < 50 ) {        //btn right
    if ( TCNT2 >= 16) {
      count++;                                            //debounces the button 
      TCNT2 = 0;

    } if (count == 40) {
      stepsleft = stepsleft - 100;                          //decrease number of steps 
      count = 0;
      if (stepsleft == 4097) {
        lcd.clear();
        stepsleft = 0;

      }
    }
    if (stepsleft == -4097) {
      lcd.clear();
      stepsleft = 0;
    }

   

  }
  if (lcd_key < 175 && lcd_key > 50) {
    lcd.clear();                        //btnUP                           //resets number of steps 
    stepsleft = 0;
  }//return stepsleft;



  if (lcd_key < 340 && lcd_key > 200) { // btnDOWN


    lcd.clear();
    low();                                                        //calls the function to move the motor based on the steps 
  }


}


void low() {

  while (stepsleft > 0 ) {
                                                //check the number of steps that has been set 

    if ( TCNT2 >= 40) {
      SetDirection();
      x = 1;
      stepper ();                       //gives the pulse and move the and it will decrease the number of steps until it stops
      stepsleft--;

      lcd.setCursor(0, 0);
      lcd.print("STEPS");

      lcd.setCursor(0, 1);
      lcd.print(stepsleft--);               //prints the as the steps decrease 


      TCNT2 = 0;

    }


    lcd.setCursor(0, 0);
    lcd.print("STEPS");

    lcd.setCursor(0, 1);                        //prints steps again 
    lcd.print(stepsleft);


  }
  while (stepsleft < 0) {                             //check the number of steps 
    if ( TCNT2 >= 40) {
      SetDirection();
      x = 1;                                        // makes steps incremant  until it stops
      stepper ();
      stepsleft++;

      TCNT2 = 0;


      lcd.setCursor(0, 0);
      lcd.print("STEPS");

      lcd.setCursor(0, 1);                          //print the increment pediocally of the steps in the above function
      lcd.print(stepsleft++);


    }


    lcd.setCursor(0, 0);
    lcd.print("STEPS");                          //prints steps again 

    lcd.setCursor(0, 1);
    lcd.print(stepsleft);

  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("STEPS");

  lcd.setCursor(0, 1);                     //prints steps again 
  lcd.print(stepsleft);


}



void linki() {
  distance();                                   //calls the function sensor 

  angle =  avg * 360 / 4096;                                // makes the conversion rate 


  if (avg >= 150) {
    stepsleft == 4096;
  }
  if (!((round(27 * (avg + 3)) > stepsleft) && (round(27 * (avg - 3)) < stepsleft))) {              //condition that will round the value based on the ratio of ditance and steps  and will check the distance reading and if  gets true then it will run the function
    if (avg * 27 < stepsleft && stepsleft > 0) {                                                      // if the condition doesnt get true the motor wont move 
      x = 0;
      SetDirection();
      stepsleft--;                                                                                // will check whether the distance decreases  and will move  CW towards 360  degress 
      stepper();
      if (stepsleft == -4097) {
        stepsleft = 0;                                                                                // check that it wont move the specific range of 0 and 360 
      }
    }
    else if (avg * 27 > stepsleft && stepsleft < 4096) {                                         // will check whether the distance increase  and will move  CCW towards 0  degress 
      x = 1;
      stepsleft++;
      stepper();
      SetDirection();
      if (stepsleft == 4097) {
        stepsleft = 0;                                                                                // check that it wont move the specific range of 0 and 360 
      }

    }
  }


}




void loop() {
  // put your main code here, to run repeatedly:


  read_LCD_buttons();                                                           //contantly check the mode function and if the select button gets pressed 



  ADCSRA |= (1 << ADSC);   //<---- Start ADC conversion bit
  while (ADCSRA != ADCSRA & ~(1 << ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
    ADCSRA &= ~(1 << ADIF);


}


ISR(TIMER1_COMPA_vect) {
  update_clock();                                       //keeps running the clock mode on  the background 

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

