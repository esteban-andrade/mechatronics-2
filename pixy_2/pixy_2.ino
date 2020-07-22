


#include <Servo.h>

Servo s1;

const int pingPin=7;
long duration;
int stopmotor=4;

const int nearend=6;
const int farend=5;
const int pwmA=3;
const int dirA=12;

int val=0;
int val1=0;
int test=0;

void setup () {
  
  pinMode(stopmotor,OUTPUT);
  digitalWrite (stopmotor,LOW);
  
  s1.attach(2);
  pinMode (dirA,OUTPUT);
  pinMode (nearend,INPUT);
  pinMode (farend,INPUT);
  digitalWrite(dirA,HIGH);
  analogWrite(pwmA,0);
  s1.write(30);
}
  
  void loop () {
    
    digitalWrite (stopmotor,LOW);
    delay(10);
    pinMode (pingPin,OUTPUT);
    digitalWrite (pingPin,LOW);
    delayMicroseconds (2);
    digitalWrite (pingPin,HIGH);
    delayMicroseconds(5);
    digitalWrite (pingPin,LOW);
    pinMode (pingPin,INPUT);
    duration=pulseIn (pingPin,HIGH);
    if (duration<500) {
      
      digitalWrite(stopmotor,HIGH);
    
    val=digitalRead(nearend);
    val1=digitalRead(farend);
    
    while (val==LOW and val1==LOW) {
      digitalWrite (dirA,LOW);
      analogWrite (pwmA,120);
      val=digitalRead(nearend);
      val1=digitalRead(farend);
    }
    if (val==HIGH) {
      digitalWrite (dirA,HIGH);
      analogWrite (pwmA,0);
      
     
      analogWrite (pwmA,150);
      delay (1500);
      s1.write(140);
    }
    
    val=digitalRead(nearend);
    val1=digitalRead(farend);
    while (val==LOW and val1==LOW) {
      val=digitalRead(nearend);
      val1=digitalRead(farend);
    }
      
    if (val1==HIGH) {
      digitalWrite (dirA,LOW);
      analogWrite (pwmA,0);
      s1.write (30);
      delay (2000);
      analogWrite (pwmA,150);
      delay (2000);
      val=digitalRead(nearend);
      val1=digitalRead(farend);
    }
    
    while (val==LOW and val1==LOW) {
      val=digitalRead(nearend);
      val1=digitalRead(farend);
    }
     if (val==HIGH) {
      digitalWrite (dirA,HIGH);
      analogWrite (pwmA,0);
      s1.write (0);
      
      delay (2000);}
    }}
