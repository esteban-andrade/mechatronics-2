//#include <FastLED.h>
//#include <Adafruit_PWMServoDriver.h> // ARMS
//#include <Wire.h>
//#include <Pixy2.h>    //CAMERA
//#include <SPI.h>
#include <LiquidCrystal.h>
//#include <PIDLoop.h>

LiquidCrystal lcd(52, 50, 42, 44, 46, 48);
//Pixy2 pixy;
//PIDLoop panLoop(400, 0, 400, true);       //sets the PID to ensure that we get accurate results
//PIDLoop tiltLoop(500, 0, 500, true);

//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
/*
//ARMS
#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             51
//MOTOR
#define enA 4
#define enB 5
#define in1 22
#define in2 24
#define in3 23
#define in4 25
//LED
#define NUM_LEDS 12
#define LED_PIN 6

CRGB leds[NUM_LEDS];

int pos = 0;
volatile int rpmcount0 = 0;
int rpm0 = 0;
unsigned long lastmillis0 = 0;
volatile int rpmcount1 = 0;
int rpm1 = 0;
unsigned long lastmillis1 = 0;
double sensor;
//int lcd_key;*/

void setup() {
  lcd.begin(16, 2);
  /*FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  ADMUX |= (1 << REFS0);
  ADMUX &= ~(1 << REFS1);
  ADMUX &= ~(1 << ADLAR);
  ADCSRA |= (1 << ADEN) |  (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRA |= (1 << ADIE);
  ADCSRA |= (1 << ADSC);
  pixy.init();
  pixy.setLamp(1, 1);
  pixy.setServos(500, 200);*/

}

void loop(){
  lcd.print("ASHER YA kuherg");
}
/*
void colourDetection() {
  ADCSRA |= (1 << ADSC);   //<---- Start ADC conversion bit
  while (ADCSRA != ADCSRA & ~(1 << ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
    ADCSRA &= ~(1 << ADIF);

  static int i;
  int j;
  char buf[64];
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {       //if camera detects objects
    i++;                            // increase frames
    if (i % 60 == 0)                  // frames of reading
      for (j = 0; j < pixy.ccc.numBlocks ; j++) {         // loop of frames for detection
        if (pixy.ccc.blocks[j].m_signature == 1) {      //  set for sigbature 1      blue
          fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0)); //BLUE
          FastLED.show();// do something for object 1
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("blue ");
        }
        if (pixy.ccc.blocks[j].m_signature == 2) {        //  set for signature 2      pink
          fill_solid(leds, NUM_LEDS, CRGB(255, 30, 0));
          FastLED.show();// do somethings for object 2
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("pink ");
        }
        if (pixy.ccc.blocks[j].m_signature == 3) {         // set for signature 3    orange
          fill_solid(leds, NUM_LEDS, CRGB(255, 0, 30 ));
          FastLED.show();// do something for object 3
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("orange ");
        }
           if (pixy.ccc.blocks[j].m_signature == 4) {    // set fot signature 4       green
             fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0));
             FastLED.show();// do something  for signaure 4
             lcd.clear();
             lcd.setCursor(0, 0);
             lcd.print("green  "); // change to green
           }
      }
  }
}

void searchMode() {
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i - 2].setRGB(0, 0, 0);
    leds[i].setRGB(255, 255, 255);
    FastLED.show();
    delay(100);
  }
}

void pinkBlink() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 102, 255));
  delay(500);
  FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  delay(500);
  FastLED.show();
}

void blueBlink() {
  fill_solid(leds, NUM_LEDS, CRGB(102, 178, 255));
  delay(500);
  FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  delay(500);
  FastLED.show();
}

void yellowBlink() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 255, 102));
  delay(500);
  FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  delay(500);
  FastLED.show();
}

void orangeBlink() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 153, 51));
  delay(500);
  FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  delay(500);
  FastLED.show();
}

void rainbowBlink() {
  fill_rainbow(leds, NUM_LEDS, 0 , 40);
  delay(500);
  FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  delay(500);
  FastLED.show();
}

int encoderA() {
  if (millis() - lastmillis0 == 500) { 
    detachInterrupt(0); //Disable interrupt when calculating

    rpm0 = rpmcount0 * 60; 
    Serial.println("Motor A");
    Serial.print("RPM =\t"); //print the word "RPM" and tab.
    Serial.print(rpm0); // print the rpm value.
    Serial.print("\t Hz=\t"); //print the word "Hz".
    Serial.println(rpmcount0); 

    rpmcount0 = 0; // Restart the RPM counter
    lastmillis0 = millis(); // Uptade lasmillis
    attachInterrupt(0, rpm_fan0, FALLING); //enable interrupt
  }
}


int encoderB() {
  if (millis() - lastmillis1 == 600) { 
    detachInterrupt(1); //Disable interrupt when calculating

    rpm1 = rpmcount1 * 60; 
    Serial.println("Motor B");
    Serial.print("RPM =\t"); //print the word "RPM" and tab.
    Serial.print(rpm1); // print the rpm value.
    Serial.print("\t Hz=\t"); //print the word "Hz".
    Serial.println(rpmcount1); 

    rpmcount1 = 0; // Restart the RPM counter
    lastmillis1 = millis(); // Uptade lasmillis
    attachInterrupt(1, rpm_fan1, FALLING); //enable interrupt
  }
}

void straight() {
  analogWrite(enA, 200); // A - 118, B - 70
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enB, 200);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void rotateCW() { // Rotate Clockwise
  analogWrite(enA, 200); // A - 118, B - 70
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enB, 200);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void rotateCCW() {  // Rotate Anti-Clockwise
  analogWrite(enA, 200); // A - 118, B - 70
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enB, 200);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void STOP() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void rpm_fan0() { 
  rpmcount0++;
}

void rpm_fan1() { 
  rpmcount1++;
}

int pulseWidth(int angle) {
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}

void dance() {
  searchMode();
  pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
  pwm.setPWM(8, 0, pulseWidth(90)); //180 - 90
  delay(400);
  pwm.setPWM(0, 0, pulseWidth(90));
  pwm.setPWM(8, 0, pulseWidth(180));
  delay(400);
  rotateCW();
  delay(200);
  rotateCCW();
  delay(400);
  rotateCW();
  delay(200);
  STOP();
}

*/
/*
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

}*/



