#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <Servo.h>
#include <Pixy2.h>    //CAMERA
#include <SPI.h>
#include <PIDLoop.h>

#define caldistance 24
#define IR_1 14
#define IR_2 15
#define IR_3 16
#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

Servo myservo;  // create servo object to control a servo
Pixy2 pixy;
PIDLoop panLoop(400, 0, 400, true);       //sets the PID to ensure that we get accurate results
PIDLoop tiltLoop(500, 0, 500, true);
Servo servoSort;

int Received = 0;
int calwidth = 40;
int calheight = 29;
int pixelswidth;
int pixelsheight;
float distancewidth;
float distanceheight;
float widthball = 4.0;
float heightball = 4.0;
int focalwidth;
int focalheight;
float avg;
int cm;
int incByte = 0;

boolean IR1 = LOW;
boolean IR2 = LOW;
boolean IR3 = LOW;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//formula= focalwight = (pixels*knowndistancecm)/widthobject
//distance=(widthball * focalwidth)/pixelwidth

void setup() {
  // put your setup code here, to run once:
  myservo.attach(2);
  Serial.begin(9600);
  servoSort.attach(17);
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(IR_3, INPUT);
  pixy.init();
  pixy.setLamp(0, 0);
  pixy.setServos(500, 750);
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  focalwidth = (calwidth * caldistance) / widthball;
  focalheight = (calheight * caldistance) / heightball;
}


void blueSlot() {
  if (IR1 == HIGH && IR2 == LOW && IR3 == LOW) {
    servoSort.write(90);
    delay(500);
    servoSort.write(83);
    delay(200);
  }
}

void pinkSlot() {
  if (IR2 == HIGH && IR1 == LOW && IR3 == LOW) {
    servoSort.write(90);
    delay(500);
    servoSort.write(83);
    delay(200);
  }
}

void greenSlot() {
  if (IR2 == HIGH && IR1 == LOW && IR3 == LOW) {
    servoSort.write(90);
    delay(500);
    servoSort.write(83);
    delay(200);
  }
}

void orangeSlot() {
  if (IR2 == LOW && IR1 == LOW && IR3 == LOW) {
    servoSort.write(90);
    delay(500);
    servoSort.write(83);
    delay(200);
  }
}


void sorting() {
  while (incByte == 103) {        // green
    servoSort.write(85);
    IR1 = digitalRead(IR_1);

    if (IR1 == HIGH) {
      servoSort.write(95);
      incByte = 0;

      break;
    }
  }
  while (incByte == 98) { //blue
    servoSort.write(85);
    IR2 = digitalRead(IR_2);

    if (IR2 == HIGH) {
      servoSort.write(95);
      incByte = 0;

      break;
    }
  }
  while (incByte == 111) { // orange
    servoSort.write(85);
    IR3 = digitalRead(IR_3);

    if (IR3 == HIGH) {
      servoSort.write(95);
      incByte = 0;

      break;
    }
  }
  while (incByte == 112) { //pink
    servoSort.write(85);
    IR1 = digitalRead(IR_1);
    IR2 = digitalRead(IR_2);
    IR3 = digitalRead(IR_3);

    if (IR1 == LOW && IR2 == LOW && IR3 == LOW) {
      servoSort.write(95);
      incByte = 0;

      break;
    }
  }
}


void Scanning() {
  static int i;
  int j;
  char buf[64];

  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {       //if camera detects objects
    i++;                            // increase frames
    if (i % 60 == 0)                  // frames of reading
      for (j = 0; j < pixy.ccc.numBlocks ; j++) {         // loop of frames for detection
        pixelswidth = pixy.ccc.blocks[j].m_width;
        pixelsheight = pixy.ccc.blocks[j].m_height;
        distancewidth = (widthball * focalwidth) / pixelswidth;
        distanceheight = (heightball * focalheight) / pixelsheight;
        avg = (distancewidth + distanceheight) / 2;
        avg = round(avg);
        if (pixy.ccc.blocks[j].m_signature == 1) {      //  set for sigbature 1      blue

          Serial.print("BLUE BALL");
          // delay(50);
          incByte = 98;
          if (pixy.ccc.blocks[j].m_x > 190  ) {


          }
          if (pixy.ccc.blocks[j].m_x < 170   ) {

          }
          if (pixy.ccc.blocks[j].m_x > 169 && pixy.ccc.blocks[j].m_x < 191 ) {


          }
          if (avg > 15) {
            pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            delay(400);                // sets the servo position according to the scaled value

          }

          if (avg <= 15) {
            pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            delay(400);

          }
        }
        if (pixy.ccc.blocks[j].m_signature == 2) {        //  set for signature 2      pink
          Serial.print("PINK BALL");
          //delay(50);
          incByte = 112;
          if (pixy.ccc.blocks[j].m_x > 190  ) {


          }
          if (pixy.ccc.blocks[j].m_x < 170   ) {

          }
          if (pixy.ccc.blocks[j].m_x > 169 && pixy.ccc.blocks[j].m_x < 191 ) {


          }
          if (avg > 15) {
            pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            delay(400);                 // sets the servo position according to the scaled value

          }

          if (avg <= 15) {
            pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            delay(400);

          }
        }
        if (pixy.ccc.blocks[j].m_signature == 3) {         // set for signature 3    orange
          Serial.print("ORANGE BALL");
          //delay(50);
          incByte = 111;
          if (pixy.ccc.blocks[j].m_x > 190  ) {


          }
          if (pixy.ccc.blocks[j].m_x < 170   ) {

          }
          if (pixy.ccc.blocks[j].m_x > 169 && pixy.ccc.blocks[j].m_x < 191 ) {


          }
          if (avg > 15 ) {
            pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            delay(400);               // sets the servo position according to the scaled value

          }

          if (avg <= 15) {
            pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            delay(400);

          }
        }
        if (pixy.ccc.blocks[j].m_signature == 4) {    // set fot signature 4       green
          Serial.print("GREEN BALL");
          //delay(50);
          incByte = 103;
          if (pixy.ccc.blocks[j].m_x > 190  ) {


          }
          if (pixy.ccc.blocks[j].m_x < 170   ) {

          }
          if (pixy.ccc.blocks[j].m_x > 169 && pixy.ccc.blocks[j].m_x < 191 ) {


          }
          if (avg > 15) {
            pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            delay(400);                  // sets the servo position according to the scaled value

          }

          if (avg <= 15) {
            pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            delay(400);

          }
        }
      }
  }
}
int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}

void dance() {
  pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
  pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
  delay(400);
  pwm.setPWM(0, 0, pulseWidth(180));
  pwm.setPWM(8, 0, pulseWidth(0));
  delay(400);
}
void loop() {


  if (Serial.available() > 0 ) {
    Received = Serial.read();
  }

  sorting();




  if (Received == '1') {                       //DANCE FUNCTION
    Serial.print("DANCING");
    delay(800);
    dance();


  }
  if (Received == '2' ) {                   //START SCANNING
    Scanning();
  }


  if (Received == '3') {       // PLAY WALL-E soundtrack
    Serial.print("WALL-E");
    delay(800);
    servoSort.write(90);

  }

  if (Received == '4') {            //move internal servo
    Serial.print("ROTATING");
    delay(800);
    servoSort.write(88);

  }
  if (Received == '5') {     //CHILL MUSIC
    Serial.print("CHILLING");
    delay(800);
    servoSort.write(90);
  }
  if (Received == '6') {     //you know it plays thug life
    Serial.print("IM WALL-E");
    delay(800);
    servoSort.write(90);
  }
  if (Received == '7') {     // STOPS MUSIC
    Serial.print("MUSIC STOPPED");
    delay(800);
    servoSort.write(90);
  }
}
