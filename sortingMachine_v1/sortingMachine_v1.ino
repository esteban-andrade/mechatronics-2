#include <Servo.h>

#define IR_1 14
#define IR_2 15
#define IR_3 16

int incByte = 0;

boolean IR1 = LOW;
boolean IR2 = LOW;
boolean IR3 = LOW;

Servo servoSort;

void setup() {
  Serial.begin (9600);
  servoSort.attach(17);
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(IR_3, INPUT);
}

void loop() { //HIGH section is Clear
  while (Serial.available() > 0) {
    incByte = Serial.read();
   
    while (incByte == 103) {        // green 
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
    while (incByte == 98) { //blue 
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
    while (incByte == 111) { // orange 
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
    while (incByte == 112) { //pink
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

