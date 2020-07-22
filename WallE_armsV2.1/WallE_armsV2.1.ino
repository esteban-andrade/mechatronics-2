#include <Servo.h>
#define SERVO_SORT 6
#define SERVO_ARMS 5
#define SERVO_RAMP 4
#define IR_1 22
#define IR_2 23
#define IR_3 24

Servo servoArm;
Servo servoSort;
Servo servoRamp;

//SORTING MACHINE
int incByte = 0;
boolean IR1 = LOW;
boolean IR2 = LOW;
boolean IR3 = LOW;
//ARM POSITION
int armPos = 0;
//RAMP POSITION
int rampPos = 0;

void setup() {
  Serial.begin (9600);
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(IR_3, INPUT);
  servoArm.attach(SERVO_ARMS);
  servoSort.attach(SERVO_SORT);
  servoRamp.attach(SERVO_RAMP);
  armsUp();
}

void rampUp(){
  for(rampPos = 0; rampPos < 120; rampPos++){
    servoArm.write(rampPos);
    delay(20);
  }
}

void rampDown() {
  for(rampPos = 120; rampPos > 0; rampPos--){
    servoArm.write(rampPos);
    delay(20);
  }
}

void armsUp() {
  for(armPos = 0; armPos < 120; armPos++){
    servoArm.write(armPos);
    delay(20);
  }
}

void armsDown() {
  for(armPos = 120; armPos > 0; armPos--){
    servoArm.write(armPos);
    delay(20);
  }
}

void bothUp() {
  for(armPos = 0; armPos < 120; armPos++){
    servoArm.write(armPos);
    servoArm.write(rampPos);
    delay(20);
  }
}

void loop() {
  while (Serial.available() > 0) { //INITIALISES THE SERIAL MONITOR FOR INPUT
    incByte = Serial.read();  //input either g,b,p,o
    //g(green) = 103, b(blue) = 98, o(orange) = 111, p(pink) = 112
    while (incByte == 103) { //GREEN
      IR1 = digitalRead(IR_1);
      Serial.println(incByte);
      if (IR1 == HIGH) {
        servoSort.write(95);
        incByte = 0;
        rampDown();
        armsDown();
        bothUp();
        Serial.println(incByte);
        break;
      }
    }
    while (incByte == 98) { //BLUE
      pwm.setPWM(SERVO_SORT, 0, pulseWidth(85));
      IR2 = digitalRead(IR_2);
      Serial.println(incByte);
      if (IR2 == HIGH) {
        pwm.setPWM(SERVO_SORT, 0, pulseWidth(95));
        incByte = 0;
        armsDown();
        //rotateArm();
        Serial.println(incByte);
        break;
      }
    }
    while (incByte == 111) { //ORANGE
      pwm.setPWM(SERVO_SORT, 0, pulseWidth(85));
      IR3 = digitalRead(IR_3);
      Serial.println(incByte);
      if (IR3 == HIGH) {
        pwm.setPWM(SERVO_SORT, 0, pulseWidth(95));
        incByte = 0;
        armsDown();
        //rotateArm();
        Serial.println(incByte);
        break;
      }
    }
    while (incByte == 112) { //PINK
      pwm.setPWM(SERVO_SORT, 0, pulseWidth(85));
      IR1 = digitalRead(IR_1);
      IR2 = digitalRead(IR_2);
      IR3 = digitalRead(IR_3);
      Serial.println(incByte);
      if (IR1 == LOW && IR2 == LOW && IR3 == LOW) {
        pwm.setPWM(SERVO_SORT, 0, pulseWidth(95));
        incByte = 0;
        armsDown();
        //rotateArm();
        Serial.println(incByte);
        break;
      }
    }
  }
}
