#include <Pixy2.h>
#include <Servo.h>
#include <SPI.h>
#include <PIDLoop.h>
Pixy2 pixy;
Servo myservo;
Servo myservo1;
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
int incByte = 0;
//formula= focalwight = (pixels*knowndistancecm)/widthobject
//distance=(widthball * focalwidth)/pixelwidth

void setup() {
  Serial.begin(115200);
  Serial.print("Starting...\n");
  myservo.attach(5);
  myservo1.attach(7);
  pixy.init();
  focalwidth = (calwidth * caldistance) / widthball;
  focalheight = (calheight * caldistance) / heightball;
  pixy.setLamp(0, 0);
  pixy.setServos(500, 400);
}

void distance () {
  static int i;
  int j;
  char buff[64];

  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks) {

    i++;
    if (i % 5 == 0 )
      for (j = 0; j < pixy.ccc.numBlocks; j++ ) {
        pixelswidth = pixy.ccc.blocks[j].m_width;
        pixelsheight = pixy.ccc.blocks[j].m_height;
        distancewidth = (widthball * focalwidth) / pixelswidth;
        distanceheight = (heightball * focalheight) / pixelsheight;
        avg = (distancewidth + distanceheight) / 2;
        avg = round(avg);
        Serial.println(avg);
        if (avg > 18 && avg < 23) {     // angles of testing vision of camera based on colour went from 20 to 22
         // myservo.write(180);                  // sets the servo position according to the scaled value
          //myservo1.write(180);                 // call the servo ramp and arms
        }

        else {
           //myservo1.write(0);                        // arms and ramp are up
          //myservo.write(0);                  // sets the servo position according to the scaled value
         
        }
      }

  }

}

void colourDetection() {
  static int i;
  int j;
  char buf[64];
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {
    i++;
    if (i % 60 == 0)

      for (j = 0; j < pixy.ccc.numBlocks ; j++) {
        if (pixy.ccc.blocks[j].m_signature == 1) {      //  set for sigbature 1      blue
          incByte = 98;
          Serial.println("BLUE BALL");


        }
        else if (pixy.ccc.blocks[j].m_signature == 2) {        //  set for signature 2      pink
          Serial.println("PINK BALL");
          //delay(50);
          incByte = 112;
        }
        else if (pixy.ccc.blocks[j].m_signature == 3) {         // set for signature 3    orange
          Serial.println("ORANGE BALL");
          //delay(50);
          incByte = 111;
        }
        else if (pixy.ccc.blocks[j].m_signature == 4) {    // set fot signature 4  green
          Serial.println("GREEN BALL");
          //delay(50);
          incByte = 103;

        }
        else {
          Serial.print("NOTHING DETECTED");
          //delay(50);
        }
      }
  }
}
void loop() {
  colourDetection();
  distance ();
}

