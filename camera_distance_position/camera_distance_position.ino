#include <Pixy2.h>
#include <Servo.h>
#include <SPI.h>
#include <PIDLoop.h>
Pixy2 pixy;
Servo myservo; 
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

void setup() {
  Serial.begin(9600);
  Serial.print("Starting...\n");
  myservo.attach(2);
  pixy.init();
  focalwidth = (calwidth * caldistance) / widthball;
  focalheight = (calheight * caldistance) / heightball;
   pixy.setLamp(0, 0);
   pixy.setServos(500, 800);
}

void loop() {
  static int i;
  int j;
  char buff[64];

  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks) {

    i++;
    if (i % 60 == 0 )
      for (j = 0; j < pixy.ccc.numBlocks; j++ ) {
       // Serial.print("block");
       // Serial.print(j);
        pixelswidth = pixy.ccc.blocks[j].m_width;
        pixelsheight = pixy.ccc.blocks[j].m_height;
        distancewidth = (widthball * focalwidth) / pixelswidth;
        distanceheight = (heightball * focalheight) / pixelsheight;
        avg = (distancewidth + distanceheight) / 2;
        avg = round(avg);
     //   Serial.print("detected");
        //Serial.println(pixy.ccc.numBlocks);
        Serial.println(avg);
        if(avg>15){
        myservo.write(0);                  // sets the servo position according to the scaled value
     delay(15);
        }

        if(avg<=15){
          myservo.write(90);
          delay(15);
        }
      }

  }

}
