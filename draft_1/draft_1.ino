#include <Servo.h>
#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11
Servo myservo;  // create servo object to control a servo

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

}
void stepper(int xw){
  for (int x=0;x<xw;x++){
switch(Steps){
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
void SetDirection(){
if(Direction==1){ Steps++;}
if(Direction==0){ Steps--; }
if(Steps>7){Steps=0;}
if(Steps<0){Steps=7; }
}


void loop() {
 while(steps_left>0){
  currentMillis = micros();
  if(currentMillis-last_time>=1000){
  stepper(1); 
  time=time+micros()-last_time;
  last_time=micros();
  steps_left--;
  }
  }
  
   // put your main code here, to run repeatedly:
  if (Serial.available() > 0 ) {
    Received = Serial.read();
  }




  if (Received == '1') {                       //DANCE FUNCTION 
    //digitalWrite(LED_BUILTIN, HIGH);
  
      Serial.print("BLUE BALL");
      delay(1000);
    


  }
  if (Received == '2' ) {                   //START SCANNING
    //digitalWrite(LED_BUILTIN, LOW);
  
      Serial.print("PINK BALL");
      delay(1000);
  
    }



  
   if (Received =='3'){         // PLAY MUSIC 
    
      Serial.print("NOTHING DETECTED");
     delay(1000);
      
    
   }

if (Received =='4'){              //move internal servo 
  // myservo.write(0);                  // sets the servo position according to the scaled value
// delay(15);  
steps_left++;

}
     
}
