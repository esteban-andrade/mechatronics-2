#include <NewPing.h>
#include <Encoder.h>
#include <Pixy2.h>
#include <SPI.h>
#include <PIDLoop.h>
#include <Servo.h>
#include <FastLED.h>

#define DISTANCE 95

//LED
#define NUM_LEDS 12
#define LED_PIN 4

CRGB leds[NUM_LEDS];

//SORTING MACHINE
#define SERVO_SORT 6
#define SERVO_ARMS 5
#define SERVO_RAMP 7
#define IR_1 22
#define IR_2 23
#define IR_3 24
Servo servoArm;
Servo servoSort;
Servo servoRamp;
//SORTING MACHINE GLOBAL VARIABLE
int incByte = 0;
boolean IR1 = LOW;
boolean IR2 = LOW;
boolean IR3 = LOW;
//ARM POSITION
int armPos = 0;
//RAMP POSITION
int rampPos = 0;


//CAMERA
Pixy2 pixy;
PIDLoop panLoop(400, 0, 400, true);       //sets the PID to ensure that we get accurate results
PIDLoop tiltLoop(500, 0, 500, true);
#define caldistance 24
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

//MOTOR PINS
#define EN1 9
#define EN2 8
#define IN5 42
#define IN6 43
#define IN7 40
#define IN8 41
#define MAX_DISTANCE 400

//ULTRASONIC SENSOR PINS
#define TRIG_FRONT 26
#define ECHO_FRONT 27
#define TRIG_LEFT 28
#define ECHO_LEFT 29

//PID
#define PERIOD 100

double error, PID_val, errorDiff;
const double P_gain = 3;
const double D_gain = 0;
const double set_point = 42;
const int nom_speed = 80;
int left_speed, right_speed;
double leftDistance;
int counter = 0;
long position0;
static double errorLast;

//GLOBAL ULTRASONIC
float distanceFront, durationFront, distanceLeft, durationLeft;
int iterations = 5;

Encoder encoderLeft(18, 19);
NewPing sonarLeft(TRIG_LEFT, ECHO_LEFT, MAX_DISTANCE);
NewPing sonarFront(TRIG_FRONT, ECHO_FRONT, MAX_DISTANCE);

void setup() {
  pixy.init();
  focalwidth = (calwidth * caldistance) / widthball;
  focalheight = (calheight * caldistance) / heightball;
  pixy.setLamp(0, 0);
  pixy.setServos(500, 800);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  Serial.begin(9600);
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(IR_3, INPUT);
  servoArm.attach(SERVO_ARMS);
  servoSort.attach(SERVO_SORT);
  servoRamp.attach(SERVO_RAMP);
  servoArm.write(180); //UP
  servoRamp.write(0); //UP
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);
  rampDown();
}

void IRone() {
  servoSort.write(85);
  IR1 = digitalRead(IR_1);
  if (IR1 == HIGH) {
    servoSort.write(95);
  }
}

void IRtwo() {
  servoSort.write(85);
  IR2 = digitalRead(IR_2);
  Serial.println(incByte);
  if (IR2 == HIGH) {
    servoSort.write(95);
  }
}

void IRthree() {
  servoSort.write(85);
  IR3 = digitalRead(IR_3);
  Serial.println(incByte);
  if (IR3 == HIGH) {
    servoSort.write(95);
  }
}

void IRfour() {
  servoSort.write(85);
  IR1 = digitalRead(IR_1);
  IR2 = digitalRead(IR_2);
  IR3 = digitalRead(IR_3);
  Serial.println(incByte);
  if (IR1 == LOW && IR2 == LOW && IR3 == LOW) {
    servoSort.write(95);
  }
}
void rampDown() {
  for (rampPos = 0; rampPos < 139; rampPos++) {
    servoRamp.write(rampPos);
    delay(20);
  }
}

void rampUp() {
  for (rampPos = 139; rampPos > 0; rampPos--) {
    servoRamp.write(rampPos);
    delay(20);
  }
}

void armsUp() {
  for (armPos = 58; armPos < 180; armPos++) {
    servoArm.write(armPos);
    delay(20);
  }
}

void armsDown() {
  for (armPos = 180; armPos > 58; armPos--) {
    servoArm.write(armPos);
    delay(20);
  }
}

void bothUp() {
  for (armPos = 58; armPos < 180 ; armPos++) {
    servoRamp.write(191 - armPos);
    delay(20);
    if (armPos > 20) {
      servoArm.write(armPos - 20);
      Serial.println(armPos);
    }
    delay(20);
  }
}

int checkMetre() {
  long newPos0 = encoderLeft.read();
  if (newPos0 != position0) {
    position0 = newPos0;
    position0 = abs(position0);
  }
  /*if (position0 > val) {
    analogWrite(EN1, 0);
    analogWrite(EN2, 0);
    encoderLeft.write(0);
    }*/
}

int checkPosCW() {
  long newPos0 = encoderLeft.read();
  if (newPos0 != position0) {
    position0 = newPos0;
    position0 = abs(position0);
  }

  return position0;
}
void rotateNinetyCW() { //Rotates WALL-E CLOCKWISE for 90 DEGREES from checkPosCW function
  analogWrite(EN1, 120);
  analogWrite(EN2, 120);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);
  while (true) {
    if (checkPosCW() > 5619) {
      analogWrite(EN1, 0);
      analogWrite(EN2, 0);
      encoderLeft.write(0);
      break;
    }
  }
}

double calcDistanceLeft() {
  durationLeft = sonarLeft.ping_median(iterations); //change back to iterations
  distanceLeft = (durationLeft / 2) * 0.0343;
  if (distanceLeft == 0) {
    return 60;
  } else {
    return distanceLeft;
  }

}

double calcDistanceFront() {
  durationFront = sonarFront.ping_median(iterations);
  distanceFront = (durationFront / 2) * 0.0343;
  if (distanceFront == 0) {
    return 60;
  } else {
    return distanceFront;
  }
}

void stopMotor() {
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);
}

void ledOff() {
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  delay(500);
  FastLED.show();
}

void blueSolid() {
  fill_solid(leds, NUM_LEDS, CRGB(102, 178, 255));
  delay(500);
  FastLED.show();
}

void pinkSolid() {
  fill_solid(leds, NUM_LEDS, CRGB(255, 99, 71));
  delay(500);
  FastLED.show();
}

void orangeSolid() {
  fill_solid(leds, NUM_LEDS, CRGB(189, 183, 107));
  delay(500);
  FastLED.show();
}

void greenSolid() {
  fill_solid(leds, NUM_LEDS, CRGB(107, 142, 35));
  delay(500);
  FastLED.show();
}

void slowSpeed() {
  analogWrite(EN1, 60);
  analogWrite(EN2, 60);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
}

void colourDetection() {
  static int i;
  int j;
  char buf[64];
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {
    i++;
    if (i % 1 == 0)
      //      pixelswidth = pixy.ccc.blocks[j].m_width;
      //    pixelsheight = pixy.ccc.blocks[j].m_height;
      //    distancewidth = (widthball * focalwidth) / pixelswidth;
      //    distanceheight = (heightball * focalheight) / pixelsheight;
      //    avg = (distancewidth + distanceheight) / 2;
      //    avg = round(avg);
      for (j = 0; j < pixy.ccc.numBlocks ; j++) {
        if (pixy.ccc.blocks[j].m_signature == 1) {      //  set for sigbature 1      blue
          blueSolid();
          long y = pixy.ccc.blocks[j].m_y;
          IRone();
          slowSpeed();
          Serial.println(pixy.ccc.blocks[j].m_y);
          if (y > DISTANCE) {        //play values between 120 and 125
            while (1) {
              stopMotor();
              rampDown();
              pixy.ccc.getBlocks();
              rampUp();
              ledOff();
              break;
            }
          }
        }
        else if (pixy.ccc.blocks[j].m_signature == 2) {        //  set for signature 2      pink
          pinkSolid();
          stopMotor();
          long y = pixy.ccc.blocks[j].m_y;
          rampDown();
          slowSpeed();
          IRtwo();
          if (y > DISTANCE) {        //play values between 120 and 125
            while (1) {
              pixy.ccc.getBlocks();
              stopMotor();
              rampUp();
              ledOff();
              break;
            }
          }
        }
        else if (pixy.ccc.blocks[j].m_signature == 3) {         // set for signature 3    orange
          orangeSolid();
          long y = pixy.ccc.blocks[j].m_y;
          rampDown();
          slowSpeed();
          IRthree();
          if (y > DISTANCE) {        //play values between 120 and 125
            while (1) {
              pixy.ccc.getBlocks();
              stopMotor();
              rampDown();
              ledOff();
              rampUp();
              break;
            }
          }
        }
        else if (pixy.ccc.blocks[j].m_signature == 4) {    // set fot signature 4  green
          greenSolid();
          long y = pixy.ccc.blocks[j].m_y;
          rampDown();
          slowSpeed();
          IRfour();
          if (y > DISTANCE) {        //play values between 120 and 125
            while (1) {
              pixy.ccc.getBlocks();
              stopMotor();
              Serial.println(pixy.ccc.blocks[j].m_y);
              ledOff();
              rampUp();
              break;
            }
          }
        }
      }
  }
}

void loop() {
  static bool first_run = true;

  static long previous_time = millis();

  colourDetection();
  leftDistance = calcDistanceLeft();
  if (leftDistance > 80) {
    encoderLeft.write(0);
  } else {
    first_run = false;
    error = set_point - leftDistance;
    errorDiff = error - errorLast;
    PID_val = (P_gain * error) - (D_gain * errorDiff / 0.005); //

    if (error > 0) //going towards the wall
    {
      left_speed = nom_speed + PID_val;
      right_speed = nom_speed - PID_val;
      if (right_speed < 50) {
        right_speed = 70;
      }
    }
    if (error < 0) //going away from the wall
    {
      left_speed = nom_speed + PID_val;
      right_speed = nom_speed - PID_val;
      if (left_speed < 50) {
        left_speed = 70;
      }
    }
    errorLast = error;
    analogWrite(EN1, left_speed);
    analogWrite(EN2, right_speed);
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, HIGH);
    digitalWrite(IN7, LOW);
    digitalWrite(IN8, HIGH);

    while (calcDistanceFront() <= 45) {
      delayMicroseconds(3000);
      counter ++;
      encoderLeft.write(0);
      rotateNinetyCW();
    }
  }
}
