//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//

#include <SPI.h>  
#include <Pixy.h>

Pixy pixy;

 int stuff=0;
int position=150;
const int red=5;
const int white=9;
const int redled=3;
const int whiteled=8;

void setup()
{
 


  
pinMode(red, OUTPUT);
  pinMode(white, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(whiteled,OUTPUT);
  
  
  digitalWrite(redled, HIGH);//right test
  digitalWrite(whiteled,HIGH);//left test
  delay(2000);
  
  digitalWrite(redled,LOW);//right off
  digitalWrite(whiteled,LOW);//left off
  digitalWrite(red,LOW);
  digitalWrite(white,LOW);
  
 
  delay(2000);
  
  

  Serial.begin(9600);
  Serial.print("Starting...\n");
}

void loop()
{ 
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32]; 
  
  blocks = pixy.getBlocks();
 
  if (blocks)
  {
    //i++;
    
    //if (i%50==0)
    //{
      //sprintf(buf, "Detected %d:\n", blocks);
     // Serial.print(buf);
      //for (j=0; j<blocks; j++)
      {
        //sprintf(buf, "  block %d: ", j);
        //Serial.print(buf); 
        //pixy.blocks[j].print();
        
        
        if(pixy.blocks[0].x>170) {
         digitalWrite(red,HIGH);
         digitalWrite(redled,HIGH);
         delay(1000);
         digitalWrite(red,LOW);
         digitalWrite(redled,LOW);
        } 
        if(pixy.blocks[0].x<150){
          digitalWrite(white,HIGH);
          digitalWrite(whiteled,HIGH);
          delay(1000);
          digitalWrite(white,LOW);
          digitalWrite(whiteled,LOW);
        }
      if(pixy.blocks[0].x>149 && pixy.blocks[0].x<171) {
        digitalWrite(white,HIGH);
        digitalWrite(whiteled,HIGH);
        digitalWrite(red,HIGH);
        digitalWrite(redled,HIGH);
        delay(1000);
        digitalWrite(white,LOW);
        digitalWrite(whiteled,LOW);
        digitalWrite(red,LOW);
        digitalWrite(redled,LOW);
        
      }
    }
  }  
}

