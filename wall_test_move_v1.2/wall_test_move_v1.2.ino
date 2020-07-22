#include <NewPing.h>
#include <Encoder.h>
#include <Servo.h>

//MOTOR PINS
#define EN1 9
#define EN2 8
#define IN5 42
#define IN6 43
#define IN7 40
#define IN8 41
//ULTRASONIC SENSOR PINS
#define TRIG_FRONT 26
#define ECHO_FRONT 27
#define TRIG_LEFT 28
#define ECHO_LEFT 29
#define MAX_DISTANCE 400
//SERVO PINS
#define SERVO_SORT 5

Encoder encoderLeft(18, 19);
NewPing sonarFront(TRIG_FRONT, ECHO_FRONT, MAX_DISTANCE);
NewPing sonarLeft(TRIG_LEFT, ECHO_LEFT, MAX_DISTANCE);
Servo servoArm;
//GLOBAL ULTRASONIC
float distanceFront, durationFront, distanceLeft, durationLeft;
int iterations = 5;
//GLOBAL ENCODERS
long position0;

void setup() {
  Serial.begin (9600);
  //SERVOS
  servoArm.attach(5);
  //MOTORS
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  servoArm.write(120); //150 - HANDS UP , 0 - HANDS DOWN
}
/*<<<<<<<<<<<<< What Motor Rotation is Chosen >>>>>>>>>>>>>>>>>*/
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

/*The Drive Function ---->>> USE THIS TO CONTROL THE MOTORS*/
void drive(int directionIn, int motorSpeed) {
  setDirection(directionIn);
  analogWrite(EN1, motorSpeed);
  analogWrite(EN2, motorSpeed);
}
/*Calculate the Distance for the FRONT Ultrasonic Sensor*/
double calcDistanceFront() {
  durationFront = sonarFront.ping_median(iterations);
  distanceFront = (durationFront / 2) * 0.0343;
  if (distanceFront == 0) {
    Serial.println("INVALID");
  } else {
    Serial.print(distanceFront);
    Serial.println(" cm");
    return distanceFront;
  }

}
/*Calculate the Distance for the LEFT Ultrasonic Sensor*/
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

boolean stopMotors() {
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);
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
  while (checkPosCW() > 3746) {
    stopMotors();
    if (stopMotors()) {
      position0 = 0;
      return;
    }
  }
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
  while (checkPosCCW() > 3746) {
    stopMotors();
    if (stopMotors()) {
      position0 = 0;
      return;
    }
  }
}

void rotateNinetyCCW1() {  //Rotates WALL-E COUNTER-CLOCKWISE for 90 DEGREES from checkPosCCW function
  drive(4, 120);
  while (true) {
    if (checkPosCCW() > 3746)
    {
      stopMotors();
      encoderLeft.write(0);
      break;
    }
  }
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

void checkMiddle() {  //WALL-E drives straight to the middle
  while (driveMiddle() > 1000) {
    stopMotors();
    if (stopMotors()) {
      position0 = 0;
      return;
    }
  }
}

void loop() {
  rotateNinetyCCW1();
  delay(1000);
  /*
  if (calcDistanceLeft() < 35) { //IF WALL-E gets to close to the left boost left motor
    analogWrite(EN1, 150);
    analogWrite(EN2, 100);
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, HIGH);
    digitalWrite(IN7, LOW);
    digitalWrite(IN8, HIGH);
  } else if (calcDistanceLeft() > 38 && calcDistanceLeft() < 70) { //IF WALL-E gets further away from the left it boosts right motor
    analogWrite(EN1, 100);
    analogWrite(EN2, 150);
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, HIGH);
    digitalWrite(IN7, LOW);
    digitalWrite(IN8, HIGH);
  } else if (calcDistanceLeft() < 38 && calcDistanceLeft() > 35) { // IF in between it drives straight
    drive(1, 100);
  }
  while (calcDistanceFront() <= 50) { //IF the distance from the front is less than 50cm it will rotate CLOCKWISE
    rotateNinetyCW();//rotateNinetyCW();
  }
  
  /*
    if (calcDistanceLeft() < 35) { //IF WALL-E gets to close to the left boost left motor
    analogWrite(EN1, 150);
    analogWrite(EN2, 100);
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, HIGH);
    digitalWrite(IN7, LOW);
    digitalWrite(IN8, HIGH);
    } else if (calcDistanceLeft() > 38 && calcDistanceLeft() < 70) { //IF WALL-E gets further away from the left it boosts right motor
    analogWrite(EN1, 100);
    analogWrite(EN2, 150);
    digitalWrite(IN5, LOW);
    digitalWrite(IN6, HIGH);
    digitalWrite(IN7, LOW);
    digitalWrite(IN8, HIGH);
    } else if (calcDistanceLeft() < 38 && calcDistanceLeft() > 35) { // IF in between it drives straight
    drive(1, 100);
    } else if (calcDistanceFront() <= 50) { //IF the distance from the front is less than 50cm it will rotate CLOCKWISE
    rotateNinetyCW();
    } else if (calcDistanceLeft() > 100 || calcDistanceLeft() == 0 ) { //FOR THE T JUNCTION
    checkMiddle();
    rotateNinetyCCW();
    }*/
  
}



