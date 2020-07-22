#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

#define IR_1 22
#define IR_2 23
#define IR_3 24
#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

int incByte = 0;
boolean IR1 = LOW;
boolean IR2 = LOW;
boolean IR3 = LOW;

Servo servoSort;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin (9600);
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(IR_3, INPUT);
}

void loop() { //HIGH section is Clear
  while (Serial.available() > 0) {
    incByte = Serial.read();
    IR3 = digitalRead(IR_3);
    
    while (incByte == 103) {
      servoSort.write(85);
      IR1 = digitalRead(IR_1);
      Serial.println(incByte);
      if (IR1 == HIGH) {
        servoSort.write(95);
        incByte = 0;
        Serial.println(incByte);
        break;
      }
    }
    while (incByte == 98) {
      servoSort.write(85);
      IR2 = digitalRead(IR_2);
      Serial.println(incByte);
      if (IR2 == HIGH) {
        servoSort.write(95);
        incByte = 0;
        Serial.println(incByte);
        break;
      }
    }
    while (incByte == 111) {
      servoSort.write(85);
      IR3 = digitalRead(IR_3);
      Serial.println(incByte);
      if (IR3 == HIGH) {
        servoSort.write(95);
        incByte = 0;
        Serial.println(incByte);
        break;
      }
    }
    while (incByte == 112) {
      servoSort.write(85);
      IR1 = digitalRead(IR_1);
      IR2 = digitalRead(IR_2);
      IR3 = digitalRead(IR_3);
      Serial.println(incByte);
      if (IR1 == LOW && IR2 == LOW && IR3 == LOW) {
        servoSort.write(95);
        incByte = 0;
        Serial.println(incByte);
        break;
      }
    }
    
    /*while (IR2 == HIGH) {
      servoSort.write(90);
      delay(1000);
      servoSort.write(80);
      delay(1000);
      break;
      }
      while (IR3 == HIGH) {
      servoSort.write(90);
      delay(1000);
      servoSort.write(80);
      delay(1000);
      break;
      }

      servoSort.write(90);
      delay(1000);
      servoSort.write(80);
      delay(500);*/

  }
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

