#include <Pixy2.h>
#include <SPI.h>
#include <PIDLoop.h>

//MOTOR PINS
#define EN1 9
#define EN2 8
#define IN5 42
#define IN6 43
#define IN7 40
#define IN8 41

#define caldistance 24
#define IR_1 14
#define IR_2 15
#define IR_3 16
#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

//PIXY CAMERA CONSTANTS
// create servo object to control a servo
Pixy2 pixy;
PIDLoop panLoop(400, 0, 400, true);       //sets the PID to ensure that we get accurate results
PIDLoop tiltLoop(500, 0, 500, true);


//PIXY CAMERA GLOBAL VARIABLES
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

//<<<<<<<<<<<PIXY CAMERA FORMULAS>>>>>>>>>>>
//formula= focalwight = (pixels*knowndistancecm)/widthobject
//distance=(widthball * focalwidth)/pixelwidth



void setup() {
  Serial.begin(9600);
  //PIXY CAMERA
  pixy.init();
  pixy.setLamp(1, 1);
  pixy.setServos(500, 700);
  //pwm.begin();
  //pwm.setPWMFreq(FREQUENCY);
  focalwidth = (calwidth * caldistance) / widthball;
  focalheight = (calheight * caldistance) / heightball;
  //MOTOR
  pinMode(EN1, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);


}

void scanning() {
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

          Serial.println(avg);
          // delay(50);
          //incByte = 98;
          if (avg < 30) { //motors stop    servo arms and ramp go down
            //            pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            //            pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            //            delay(400);//
            drive(1, 0);

          }else if (pixy.ccc.blocks[j].m_x > 190  ) { //object on the right of the plane
            drive(3, 80);

          }else if (pixy.ccc.blocks[j].m_x < 170   ) {   //object on the left of the plane
            drive(4, 80);
          }else if (pixy.ccc.blocks[j].m_x > 169 && pixy.ccc.blocks[j].m_x < 191 ) {      // object on the middle of the plane
            drive(1, 80);

          }else if (avg > 26) {           // motors drive  and arms and ramp up
            //pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            //pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            //delay(400);                // sets the servo position according to the scaled value

          }


        }
      }
  }
}

void setDirection(int directionIn) {
  switch (directionIn) {
    case 1: //Forward
      {
        digitalWrite(IN5, HIGH);
        digitalWrite(IN6, LOW);
        digitalWrite(IN7, HIGH);
        digitalWrite(IN8, LOW);
        break;
      }
    case 2: //Backwards
      {
        digitalWrite(IN5, LOW);
        digitalWrite(IN6, HIGH);
        digitalWrite(IN7, LOW);
        digitalWrite(IN8, HIGH);
        break;
      }
    case 3: //Left (Right motor forward & left motor backwards)
      {
        digitalWrite(IN5, LOW);
        digitalWrite(IN6, HIGH);
        digitalWrite(IN7, HIGH);
        digitalWrite(IN8, LOW);
        break;
      }
    case 4: //Right (Left motor forward & right motor backwards)
      {
        digitalWrite(IN5, HIGH);
        digitalWrite(IN6, LOW);
        digitalWrite(IN7, HIGH);
        digitalWrite(IN8, LOW);
        break;
      }
    case 5:
      {
        analogWrite(EN1, 0);
        analogWrite(EN2, 0);
      }
  }
}

void drive(int directionIn, int motorSpeed) {
  setDirection(directionIn);
  analogWrite(EN1, motorSpeed);
  analogWrite(EN2, motorSpeed);

}

void loop() {
  scanning();

}
