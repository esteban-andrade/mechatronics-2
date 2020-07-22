 int mg2 = 4;
int mg1 = 5;
int md1 = 3;
int md2 = 2;
int enag= 6;
#include <Wire.h>
#include <Pixy.h>


Pixy pixy;

void setup() {
   pinMode(enag, OUTPUT);
      pinMode(md1, OUTPUT);
        pinMode(md2, OUTPUT);
        pinMode(mg1, OUTPUT);
        pinMode(mg2, OUTPUT);
 Serial.begin(9600);
  Serial.print("Starting...\n");
  
  pixy.init();

}



void loop() { 
static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32]; 
  
  blocks = pixy.getBlocks();
  
  if (blocks)
  {
    i++;
    
    // do this (print) every 50 frames because printing every
    // frame would bog down the Arduino
    if (i%50==0)
    {
      sprintf(buf, "Detected %d:\n", blocks);
      Serial.print(buf);
      for (j=0; j<blocks; j++)
      {
        sprintf(buf, "  block %d: ", j);
        Serial.print(buf); 
        pixy.blocks[j].print();
 
      }
  
    }
           if((pixy.blocks[0].x>120)&&(pixy.blocks[0].x<190)) {
  go();}
 if((pixy.blocks[0].x<120) && (pixy.blocks[0].x>10)){
  gauche();
  }
  if(pixy.blocks[0].x>190) {
  droite();}
  
  } 
else{o9if();};

}
 

 


  
/*  else{o9if();
  delay(2000);}
  */




void droite(){
          analogWrite(enag,210);
          analogWrite(mg1,0);
          analogWrite(mg2,0);
          analogWrite(md1,255);
          analogWrite(md2,0);  
            delay(30);}
void gauche(){
          analogWrite(enag,210);
          analogWrite(mg1,255);
          analogWrite(mg2,0);
          analogWrite(md1,0);
          analogWrite(md2,0);
            delay(30);}
void o9if(){
          analogWrite(mg1,0);
          analogWrite(mg2,0);
          analogWrite(md2,0);
          analogWrite(md1,0);
         delay(30); }
void go (){
          analogWrite(enag,210);
          analogWrite(mg1,255);
          analogWrite(mg2,0);
          analogWrite(md1,255);
          analogWrite(md2,0);
         delay(30); }
