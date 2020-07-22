#include <Pixy2.h>
#include <Servo.h>
#include<SPI.h>
Pixy2 pixy;
Servo myservo;
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
  myservo.attach(7);
  pixy.init();
  pixy.setServos(500, 620);
  focalwidth = (calwidth * caldistance) / widthball;
  focalheight = (calheight * caldistance) / heightball;

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
        if (pixy.ccc.blocks[j].m_signature == 1) {      //  set for sigbature 1      blue
          // do something for object 1
          ;
        }
        else if (pixy.ccc.blocks[j].m_signature == 2) {        //  set for signature 2      pink
          // do somethings for object 2

        }
        else if (pixy.ccc.blocks[j].m_signature == 3) {         // set for signature 3    orange
          // do something for object 3

        }
        else if (pixy.ccc.blocks[j].m_signature == 4) {    // set fot signature 4       yellow
          // do something  for signaure 4

        }
        else if (avg<25 ){   // distance to call arms and ramp to be less than 24
          //stop driving  and call servo for ramp and arms

        }
        else {
          
        }

       

      }

  }

}
