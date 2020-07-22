#include <Servo.h>
#include <Pixy2.h>    //CAMERA
#include <SPI.h>
#include <PIDLoop.h>
#define IN1  4
#define IN2  5
#define IN3  6
#define IN4  7
#define caldistance 24
Servo myservo;  // create servo object to control a servo
Pixy2 pixy;
PIDLoop panLoop(400, 0, 400, true);       //sets the PID to ensure that we get accurate results
PIDLoop tiltLoop(500, 0, 500, true);
int Received = 0;
int Steps = 0;
int Direction;
unsigned long last_time;
unsigned long currentMillis ;
int steps_left;
long time;

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

//formula= focalwight = (pixels*knowndistancecm)/widthobject
//distance=(widthball * focalwidth)/pixelwidth

void setup() {
  // put your setup code here, to run once:
  myservo.attach(2);
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pixy.init();
  pixy.setLamp(0, 0);
  pixy.setServos(500, 800);
  focalwidth = (calwidth * caldistance) / widthball;
  focalheight = (calheight * caldistance) / heightball;
}
void stepper(int xw) {
  for (int x = 0; x < xw; x++) {
    switch (Steps) {
      case 0:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        break;
      case 1:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, HIGH);
        break;
      case 2:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        break;
      case 3:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        break;
      case 4:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
      case 5:
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
      case 6:
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
      case 7:
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        break;
      default:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
    }
    SetDirection();
  }
}
void SetDirection() {
  if (Direction == 1) {
    Steps++;
  }
  if (Direction == 0) {
    Steps--;
  }
  if (Steps > 7) {
    Steps = 0;
  }
  if (Steps < 0) {
    Steps = 7;
  }
}


void loop() {

  static int i;
  int j;
  char buf[64];
  while (steps_left > 0) {
    currentMillis = micros();
    if (currentMillis - last_time >= 1000) {
      stepper(1);
      time = time + micros() - last_time;
      last_time = micros();
      steps_left--;
    }
  }



  if (Serial.available() > 0 ) {
    Received = Serial.read();
  }




  if (Received == '1') {                       //DANCE FUNCTION
    Serial.print("DANCING");
    delay(800);
    myservo.write(90);
    delay(100);
    myservo.write(0);
    delay(100);
    myservo.write(180);



  }
  if (Received == '2' ) {                   //START SCANNING
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
            if (pixy.ccc.blocks[j].m_x > 191  ) {
              steps_left = 1096;
              Direction = 1;

            }
            if (pixy.ccc.blocks[j].m_x < 169   ) {
              steps_left = 1096;
              Direction = 0;
            }
            if (pixy.ccc.blocks[j].m_x > 170 && pixy.ccc.blocks[j].m_x < 190 ) {
              steps_left = 0 ;

            }
            if (avg > 15) {
              myservo.write(0);                  // sets the servo position according to the scaled value

            }

            if (avg <= 15) {
              myservo.write(90);

            }
          }
          if (pixy.ccc.blocks[j].m_signature == 2) {        //  set for signature 2      pink
            Serial.print("PINK BALL");
            //delay(50);
            if (pixy.ccc.blocks[j].m_x > 191  ) {
              steps_left = 1096;
              Direction = 1;

            }
            if (pixy.ccc.blocks[j].m_x < 169   ) {
              steps_left = 1096;
              Direction = 0;
            }
            if (pixy.ccc.blocks[j].m_x > 170 && pixy.ccc.blocks[j].m_x < 190 ) {
              steps_left = 0 ;

            }
            if (avg > 15) {
              myservo.write(0);                  // sets the servo position according to the scaled value

            }

            if (avg <= 15) {
              myservo.write(90);

            }
          }
          if (pixy.ccc.blocks[j].m_signature == 3) {         // set for signature 3    orange
            Serial.print("ORANGE BALL");
            //delay(50);
            if (pixy.ccc.blocks[j].m_x > 191  ) {
              steps_left = 1096;
              Direction = 1;

            }
            if (pixy.ccc.blocks[j].m_x < 169   ) {
              steps_left = 1096;
              Direction = 0;
            }
            if (pixy.ccc.blocks[j].m_x > 170 && pixy.ccc.blocks[j].m_x < 190 ) {
              steps_left = 0 ;

            }
            if (avg > 15 ) {
              myservo.write(0);                  // sets the servo position according to the scaled value

            }

            if (avg <= 15) {
              myservo.write(90);

            }
          }
          if (pixy.ccc.blocks[j].m_signature == 4) {    // set fot signature 4       green
            Serial.print("GREEN BALL");
            //delay(50);
            if (pixy.ccc.blocks[j].m_x > 191  ) {
              steps_left = 1096;
              Direction = 1;

            }
            if (pixy.ccc.blocks[j].m_x < 169   ) {
              steps_left = 1096;
              Direction = 0;
            }
            if (pixy.ccc.blocks[j].m_x > 170 && pixy.ccc.blocks[j].m_x < 190 ) {
              steps_left = 0 ;

            }
            if (avg > 15) {
              myservo.write(0);                  // sets the servo position according to the scaled value

            }

            if (avg <= 15) {
              myservo.write(90);

            }
          }
        }
    }
  }




  if (Received == '3') {       // PLAY MUSIC
    Serial.print("MUSIC");
    delay(800);


  }

  if (Received == '4') {            //move internal servo
    // myservo.write(0);                  // sets the servo position according to the scaled value
    // delay(15);
    steps_left = 1096;
    Direction = 1;

  }

}
