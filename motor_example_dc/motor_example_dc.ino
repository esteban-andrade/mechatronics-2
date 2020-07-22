int val=0;
int val1=0;
int stopval=0;
 int valm=0;
 int valm1=0;
 int jump=1;
 
 int stoppin=4;
const int pwmA=3; 
const int pwmB=11;
 const int brakeA=9;
 const int brakeB=8;
 const int dirA=12;
 const int dirB=13;
 const int right=5; 
const int left=7;
void setup() {
  pinMode (stoppin,INPUT);
  
 pinMode(dirA, OUTPUT);
 pinMode(brakeA, OUTPUT);
 pinMode(dirB, OUTPUT); 
pinMode(brakeB, OUTPUT); 
pinMode(right,INPUT);
 pinMode(left,INPUT);
 digitalWrite(dirA, HIGH);//forward A 
digitalWrite(brakeA,LOW);//release brakeA
 analogWrite(pwmA,100);//set speed A 
digitalWrite(dirB,HIGH);//forward B motor 
digitalWrite(brakeB,LOW);// 
analogWrite(pwmB,100);//set speed B 
delay(700);
}
void loop () { 

 //jump equal one
 analogWrite(pwmA,0);
 analogWrite(pwmB,0);
 val=digitalRead(right);
 val1=digitalRead(left);
stopval=digitalRead(stoppin);

if (stopval==HIGH) {
  analogWrite(pwmA,0);
  analogWrite(pwmB,0);
  delay(16000);
  
 
}

val=digitalRead(right);
 val1=digitalRead(left);
 
if (val==HIGH && val1==HIGH) {
 analogWrite(pwmA,90);
 analogWrite(pwmB,90); 
delay(100);
 }
 if (val==HIGH &&val1==LOW) { 
analogWrite(pwmA, 150);
 analogWrite(pwmB,30);
 delay(100);
 }
 if (val1==HIGH && val==LOW) {
 analogWrite(pwmB,150);
 analogWrite(pwmA,30);
 delay(100);
 } 
}

