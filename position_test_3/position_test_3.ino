#include<Pixy2.h>
#include <SPI.h>
#include <PIDLoop.h>
Pixy2 pixy;
PIDLoop panLoop(400, 0, 400, true);       //sets the PID to ensure that we get accurate results
PIDLoop tiltLoop(500, 0, 500, true);


void setup() {


  pixy.init();
  pixy.setServos(500, 500);
  Serial.begin(115200);
  Serial.print("Starting...\n");


}


void loop() {
  static int i;
  int j;
  char buff[64];



  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks) {
    i++;
    if (i % 1== 0)
      Serial.print("Detected ");
    //Serial.println(pixy.ccc.numBlocks);
    for (j = 0; j < pixy.ccc.numBlocks; j++) {

     // Serial.print("  block ");
      //Serial.print(j);
      //Serial.print(": ");
      pixy.ccc.blocks[j].print();
      if (pixy.ccc.blocks[j].m_y > 150  ) {

        Serial.println("upper");

      }
      if (pixy.ccc.blocks[j].m_y < 70   ) {
        Serial.println("lower");
      }
      if (pixy.ccc.blocks[j].m_x > 151 && pixy.ccc.blocks[j].m_x < 69 ) {
        Serial.println("middle");


      }
    }


  }

}
