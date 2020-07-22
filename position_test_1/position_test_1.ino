#include<Pixy2.h>
#include <SPI.h>
#include <PIDLoop.h>
Pixy2 pixy;
PIDLoop panLoop(400, 0, 400, true);       //sets the PID to ensure that we get accurate results
PIDLoop tiltLoop(500, 0, 500, true);
void setup() {
  pixy.init();
   pixy.setServos(500, 620);
   Serial.begin(115200);
  Serial.print("Starting...\n");
  pinMode(7, OUTPUT);

}

void loop() {
  static int i;
  int j;
  char buff[64];
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
      if (pixy.ccc.blocks[j].m_x >186  ){
        digitalWrite(7, LOW);
        
      }
      if(pixy.ccc.blocks[j].m_x <160   ){
        digitalWrite(7, LOW);

      }
      if (pixy.ccc.blocks[j].m_x >161 && pixy.ccc.blocks[j].m_x <185 ){
         digitalWrite(7, HIGH); 
         
      }
    }
    

  }

}
