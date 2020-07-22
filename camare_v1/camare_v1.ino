
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


void scanning() {
  static int i;
  int j;
  char buf[64];

  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {       //if camera detects objects
    i++;                            // increase frames
    if (i % 60 == 0)                  // frames of reading
      for (j = 0; j < pixy.ccc.numBlocks ; j++) {         // loop of frames for detection
        pixelswidth = pixy.ccc.blocks[j].m_width;
        pixelsheight = pixy.ccc.blocks[j].m_height;
        distancewidth = (widthball * focalwidth) / pixelswidth;
        distanceheight = (heightball * focalheight) / pixelsheight;
        avg = (distancewidth + distanceheight) / 2;
        avg = round(avg);
        if (pixy.ccc.blocks[j].m_signature == 1) {      //  set for sigbature 1      blue

          Serial.print("BLUE BALL");
          // delay(50);
          //incByte = 98;
          if (avg < 50) { //motors stop    servo arms and ramp go down
            //            pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            //            pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            //            delay(400);//
            drive(1, 0);

          }
         el if (pixy.ccc.blocks[j].m_x > 190  ) { //object on the right of the plane
            drive(3, 70);

          }
          if (pixy.ccc.blocks[j].m_x < 170   ) {   //object on the left of the plane
            drive(4, 70);
          }
          if (pixy.ccc.blocks[j].m_x > 169 && pixy.ccc.blocks[j].m_x < 191 ) {      // object on the middle of the plane
            drive(1, 70);

          }
          if (avg > 26) {           // motors drive  and arms and ramp up
            //pwm.setPWM(0, 0, pulseWidth(0));// 0 - 90
            //pwm.setPWM(8, 0, pulseWidth(180)); //180 - 90
            //delay(400);                // sets the servo position according to the scaled value

          }

          
        }
      }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
