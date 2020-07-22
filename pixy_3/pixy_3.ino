
#include <SPI.h>
#include<Pixy.h>
Pixy pixy;
int stuff=0;
 int position=150;
 const int red=5;
 const int white=9;
 const int redled=3; 
const int whiteled=8;
void setup() { 
pinMode(red, OUTPUT);
 pinMode(white, OUTPUT); 
pinMode(redled, OUTPUT);
 pinMode(whiteled,OUTPUT);
 digitalWrite(red,LOW);
 digitalWrite(white,LOW); 
 digitalWrite(redled, HIGH);//right test 
digitalWrite(whiteled,HIGH);//left test
 delay(2000); 
digitalWrite(redled,LOW);//right off 
digitalWrite(whiteled,LOW);//left off 

delay(2000); 
Serial.begin(9600);
 Serial.print("Starting...\n");
 }
void loop() {
 static int i = 0;
 int j;
 uint16_t blocks;
 char buf[32];
 blocks = pixy.getBlocks();
 if (blocks) {
 i++;
 if (i%5==0) {
 //sprintf(buf, "Detected %d:\n", blocks);
 // Serial.print(buf); //
//for (j=0; j=170;) {
 if(pixy.blocks[j].x>190){  
digitalWrite(red,HIGH);
 digitalWrite(redled,HIGH); 
delay(200); 
digitalWrite(red,LOW);
 digitalWrite(redled,LOW); 
} 
if(pixy.blocks[j].x<170){ 
digitalWrite(white,HIGH);
 digitalWrite(whiteled,HIGH);
 delay(200); 
digitalWrite(white,LOW);
 digitalWrite(whiteled,LOW); 
} 
if(pixy.blocks[j].x>169 && pixy.blocks[j].x<191) { 
digitalWrite(white,HIGH);
 digitalWrite(whiteled,HIGH);
 digitalWrite(red,HIGH);
 digitalWrite(redled,HIGH); 
delay(200); 
digitalWrite(white,LOW); 
digitalWrite(whiteled,LOW); 
digitalWrite(red,LOW);
 digitalWrite(redled,LOW); 
} } } }

