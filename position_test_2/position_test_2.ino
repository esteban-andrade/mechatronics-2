#include<Pixy2.h>
#include <SPI.h>
#include <PIDLoop.h>
Pixy2 pixy;
PIDLoop panLoop(400, 0, 400, true);       //sets the PID to ensure that we get accurate results
PIDLoop tiltLoop(500, 0, 500, true);
#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11
int Steps;
boolean Direction;
unsigned long last_time;
unsigned long currentMillis ;
int steps_left;
long time;


void setup() {
 pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  pixy.init();
   pixy.setServos(500, 700);
   Serial.begin(115200);
  Serial.print("Starting...\n");
  pinMode(7, OUTPUT);

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
  char buff[64];
 while (steps_left > 0) {
    currentMillis = micros();
    if (currentMillis - last_time >= 1000) {
      stepper(1);
      time = time + micros() - last_time;
      last_time = micros();
      steps_left--;
    }
  }

  
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks) {
    i++;
    if (i%60==0)
     Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks);
    for (j=0; j < pixy.ccc.numBlocks;j++){
      
      Serial.print("  block ");
      Serial.print(j);
      Serial.print(": ");
      pixy.ccc.blocks[j].print();
      if (pixy.ccc.blocks[j].m_x >191  ){
       steps_left = 1096;
       Direction=1;
        
      }
      if(pixy.ccc.blocks[j].m_x <169   ){
        steps_left = 1096;
        Direction=0;
      }
      if (pixy.ccc.blocks[j].m_x >170 && pixy.ccc.blocks[j].m_x <190 ){
         steps_left = 0 ;
         
      }
    }
    

  }

}
