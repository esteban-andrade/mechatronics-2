#include <Servo.h>
#include <Pixy2.h>    //CAMERA
#include <SPI.h>
#include <PIDLoop.h>
#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11
Servo myservo;  // create servo object to control a servo
Pixy2 pixy;
PIDLoop panLoop(400, 0, 400, true);       //sets the PID to ensure that we get accurate results
PIDLoop tiltLoop(500, 0, 500, true);
int Received = 0;
int Steps = 0;
boolean Direction = true;
unsigned long last_time;
unsigned long currentMillis ;
int steps_left;
long time;
void setup() {
  // put your setup code here, to run once:
  myservo.attach(7);
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pixy.init();
  pixy.setLamp(1, 1);
  pixy.setServos(500, 700);
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



  }
  if (Received == '2' ) {                   //START SCANNING
    pixy.ccc.getBlocks();

    if (pixy.ccc.numBlocks) {       //if camera detects objects
      i++;                            // increase frames
      if (i % 60 == 0)                  // frames of reading
        for (j = 0; j < pixy.ccc.numBlocks ; j++) {         // loop of frames for detection
          if (pixy.ccc.blocks[j].m_signature == 1) {      //  set for sigbature 1      blue

            Serial.print("BLUE BALL");
            // delay(50);

          }
          if (pixy.ccc.blocks[j].m_signature == 2) {        //  set for signature 2      pink
            Serial.print("PINK BALL");
            //delay(50);
          }
          if (pixy.ccc.blocks[j].m_signature == 3) {         // set for signature 3    orange
            Serial.print("ORANGE BALL");
            //delay(50);
          }
          if (pixy.ccc.blocks[j].m_signature == 4) {    // set fot signature 4       green
            Serial.print("GREEN BALL");
            //delay(50);
          }



        }
    }




  }




  if (Received == '3') {       // PLAY MUSIC
    Serial.print("IM WALL-E");
    delay(800);


  }

  if (Received == '4') {            //move internal servo
    // myservo.write(0);                  // sets the servo position according to the scaled value
    // delay(15);
    steps_left = 1096;

  }
  if (Received == '5') {
    Serial.print("Wii music");
    delay(800);
  }
  if
  (Received == '6') {
    Serial.print("THUG LIFE");
    delay(800);
  }
  if (Received == '7') {
    Serial.print("STOPPED");
    delay(800);
  }
}

