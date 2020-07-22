#include <NewPing.h>
#include <Encoder.h>

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

double error, PID_val;
const double P_gain = 9;
const double D_gain = 0.3;
const double set_point = 36;
const int nom_speed = 70;
int left_speed, right_speed;
double leftDistance;

//GLOBAL ULTRASONIC
float distanceFront, durationFront, distanceLeft, durationLeft;
int iterations = 5;

//GLOBAL ENCODERS
long position0 = -999;

Encoder encoderLeft(18, 19);
NewPing sonarLeft(TRIG_LEFT, ECHO_LEFT, MAX_DISTANCE);
NewPing sonarFront(TRIG_FRONT, ECHO_FRONT, MAX_DISTANCE);

void setup() {
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
}

double calcDistanceLeft() {
  durationLeft = sonarLeft.ping_median(iterations); //change back to iterations
  distanceLeft = (durationLeft / 2) * 0.0343;
  if (distanceLeft == 0) {
    Serial.println("INVALID");
    return 60;
  } else {
    Serial.print(distanceLeft);
    Serial.println(" cm");
    return distanceLeft;
  }

}

double calcDistanceFront() {
  durationFront = sonarFront.ping_median(iterations);
  distanceFront = (durationFront / 2) * 0.0343;
  if (distanceFront == 0) {
    Serial.println("INVALID");
    return 60;
  } else {
    Serial.print(distanceFront);
    Serial.println(" cm");
    return distanceFront;
  }
}

void setDirection(int directionIn) {
  switch (directionIn) {
    case 1: //Forward
      {
        digitalWrite(IN5, LOW);
        digitalWrite(IN6, HIGH);
        digitalWrite(IN7, LOW);
        digitalWrite(IN8, HIGH);
        break;
      }
    case 2: //Backwards
      {
        digitalWrite(IN5, HIGH);
        digitalWrite(IN6, LOW);
        digitalWrite(IN7, HIGH);
        digitalWrite(IN8, LOW);
        break;
      }
    case 3: //Left (Right motor forward & left motor backwards)
      {
        digitalWrite(IN5, HIGH);
        digitalWrite(IN6, LOW);
        digitalWrite(IN7, LOW);
        digitalWrite(IN8, HIGH);
        break;
      }
    case 4: //Right (Left motor forward & right motor backwards)
      {
        digitalWrite(IN5, LOW);
        digitalWrite(IN6, HIGH);
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

boolean stopMotors() {
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);
}

/*The Drive Function ---->>> USE THIS TO CONTROL THE MOTORS*/
void drive(int directionIn, int motorSpeed) {
  setDirection(directionIn);
  analogWrite(EN1, motorSpeed);
  analogWrite(EN2, motorSpeed);
}

int checkPosCW() {
  long newPos0 = encoderLeft.read();
  if (newPos0 != position0) {
    position0 = newPos0;
    position0 = abs(position0);
    Serial.println(position0);
  }

  return position0;
}
void rotateNinetyCW() { //Rotates WALL-E CLOCKWISE for 90 DEGREES from checkPosCW function
  drive(3, 120);
  while (true) {
    if (checkPosCW() > 5500) {
      stopMotors();
      encoderLeft.write(0);
      break;
    }
  }
  delay(1000);
  encoderLeft.write(0);
}

int checkPosCCW() {  //Rotates WALL-E COUNTER-CLOCKWISE and returns the encoder value
  long newPos0 = encoderLeft.read();
  if (newPos0 != position0) {
    position0 = newPos0;
    position0 = abs(position0);
    Serial.println(position0);
  }
  return position0;
}

void rotateNinetyCCW() {  //Rotates WALL-E COUNTER-CLOCKWISE for 90 DEGREES from checkPosCCW function
  drive(4, 120);
  while (true) {
    if (checkPosCCW() > 3746)
    {
      stopMotors();
      encoderLeft.write(0);
      break;
    }
  }
  delay(1000);
  encoderLeft.write(0);
}

int driveMiddle() {  //WALL-E drives straight and returns encoder value
  drive(1, 120);
  long newPos0 = encoderLeft.read();
  if (newPos0 != position0) {
    position0 = newPos0;
    position0 = abs(position0);
    Serial.println(position0);
  }
  return position0;
}

void checkMiddle(int val) {  //WALL-E drives straight to the middle
  while (true) {
    if (driveMiddle() > val)
    {
      stopMotors();
      break;
    }
  }
  delay(1000);
  encoderLeft.write(0);
}

/*THIS SEQUENCE STRAIGHT HALF A METRE ROTATES DRIVES STRAIGHT
  checkMiddle();
  rotateNinetyCCW();
  checkMiddle();*/

void loop() {
    leftDistance = calcDistanceLeft();
    if (leftDistance > 80) {
    encoderLeft.write(0);
    checkMiddle(15000);
    } 
    else 
    {
    error = set_point - leftDistance;
    PID_val = P_gain * error;

    if (error > 0) //going towards the wall
    {
      left_speed = nom_speed + PID_val;
      right_speed = nom_speed - PID_val;
      if(right_speed < 50) right_speed = 70;
    }
    if (error < 0) //going away from the wall
    {
      left_speed = nom_speed + PID_val;
      right_speed = nom_speed - PID_val;
      if(left_speed < 50) left_speed = 70;
    }
    analogWrite(EN1, left_speed);
    analogWrite(EN2, right_speed);
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, HIGH);
    digitalWrite(IN7, LOW);
    digitalWrite(IN8, HIGH);

        while (calcDistanceFront() <= 40) {
          delayMicroseconds(3000);
          encoderLeft.write(0);
          rotateNinetyCW();
        }
    }

}
