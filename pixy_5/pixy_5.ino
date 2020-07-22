
//jump0 charge
//jump1 bump around
//jump2 backup turn around
//jump3 find station
//jump4 bump and watch for home
//jmp5 continuation of jump1

int charge=0;
int val=0;
int val1=0;
int valm=0;
int valm1=0;
int valm3=0;
int jump=3;
int ctr=0;//counter
long int delayct=0;
long int runtime=30000;//time to run around
long int runtimehold=30000;//remember runtime
long int stuck=0;//stuck counter

const int pwmA=3;
const int pwmB=11;
const int brakeA=9;
const int brakeB=8;
const int dirA=12;
const int dirB=13;
const int right=5;
const int left=7;
const int chglt=6;

void setup() {
pinMode(dirA, OUTPUT);
  pinMode(brakeA, OUTPUT);
  pinMode(dirB, OUTPUT);
  pinMode(brakeB, OUTPUT);
  pinMode(right,INPUT);
  pinMode(left,INPUT);
  pinMode(chglt,OUTPUT);
  
  digitalWrite(dirA, HIGH);//forward A
  digitalWrite(brakeA,LOW);//release brakeA
  analogWrite(pwmA,100);//set speed A
  digitalWrite(chglt,HIGH);//test charge light
  
  digitalWrite(dirB,HIGH);//forward B motor
  digitalWrite(brakeB,LOW);//
  analogWrite(pwmB,100);//set speed B
  delay(700);
  digitalWrite(chglt,LOW);//set charge light off

}

void loop () {
  
  if (jump==0) {
    analogWrite(pwmA,0);//charge found
    analogWrite(pwmB,0);
    digitalWrite(chglt,HIGH);
   if(delayct==0) {
     delay(2000);
     delayct=1;
   }
    valm3=analogRead(3);
    if(valm3>350) {
      jump=2;
      valm3=0;
      ctr=0;
      runtime=runtimehold;//reset runtime
    }
  }
  
  if (jump==1) {
   digitalWrite(dirA,HIGH);//fwd
    digitalWrite(dirB,HIGH);
    analogWrite(pwmA,140);
    analogWrite(pwmB,140);
    delay(1000);
   jump=5;} 
    
 
  if(jump==5) {
  valm=analogRead(0);
  valm1=analogRead(1);
  val=digitalRead(right);
  val1=digitalRead(left);
  delayct=delayct+1;
  if(delayct>runtime && val==HIGH) {
    val=digitalRead(right);
    if (val==HIGH) {//make sure we're reading target
    jump=3;
    delayct=0;
  }}
  if(delayct>runtime && val1==HIGH) {
    val1=digitalRead(left);
    if (val1==HIGH) {
    jump=3;
    delayct=0;
  }}
  
  
  if(valm>500 or valm1>500) {
    
    valm=analogRead(0);
  valm1=analogRead(1);
  if(valm>450 or valm1>450) {
    digitalWrite(brakeA, HIGH);
    digitalWrite(brakeB, HIGH);
    
    digitalWrite(dirA, LOW);//reverse A
    digitalWrite(brakeA, LOW);//
    analogWrite(pwmA, 200);
    
    digitalWrite(dirB,LOW);
    digitalWrite(brakeB,LOW);
    analogWrite(pwmB,200);
    
    delay(700);//backup
    
    digitalWrite(brakeA,HIGH);//stop one wheel
    analogWrite(pwmA,0);
    
    delay(1500);
    digitalWrite(brakeB,HIGH);//stop other wheel
    
    //start both wheels forward
    
    digitalWrite(dirA,HIGH);
    digitalWrite(brakeA,LOW);
    analogWrite(pwmA,140);
    
    digitalWrite(dirB,HIGH);
    digitalWrite(brakeB,LOW);
    analogWrite(pwmB,140);
    delay(1000);
  }}}
  
  if(jump==2) {
    digitalWrite(dirA,LOW);//reverse
    digitalWrite(dirB,LOW);
    analogWrite(pwmA,200);
    analogWrite(pwmB,200);
    delay(1000);
    analogWrite(pwmA,0);
    delay(500);
    jump=1;
  }
  
  //find station
  if (jump==3) {
    
    analogWrite(pwmA,0);
    analogWrite(pwmB,0);
    
    
     val=digitalRead(right);
     val1=digitalRead(left);
  if (val==HIGH && val1==HIGH) {
    analogWrite(pwmA,140);
    analogWrite(pwmB,140);
    stuck=0;
    
    if (ctr<75) {
      ctr=ctr+1;
    charge=analogRead(2);//test for charge station
    if(charge>100) {
      jump=0;
      ctr=0;
    }
    if(ctr==75) {
      ctr=0;
    }}
  }
  
  if (val==HIGH &&val1==LOW) {
    analogWrite(pwmA, 250);
    analogWrite(pwmB,50);
    stuck=0;
    
    if (ctr<75) {
      ctr=ctr+1;
    charge=analogRead(2);//test for charge station
    if(charge>100) {
      jump=0;
      ctr=0;
    }
    if(ctr==75) {
      ctr=0;
    }}
  }
  if (val1==HIGH && val==LOW) {
  
    analogWrite(pwmB,275);
    analogWrite(pwmA,25);
    stuck=0;
    
    if (ctr<75) {
      ctr=ctr+1;
    charge=analogRead(2);//test for charge station
    if(charge>100) {
      jump=0;
      ctr=0;
    }
    if(ctr==75) {
      ctr=0;
    }}
  }
 stuck=stuck+1;
 
    if (stuck>10000000) {//no sight of home base
    digitalWrite(chglt,HIGH);
    delay(500);
    digitalWrite(chglt,LOW);
      jump=1;
      stuck=0;
      runtime=10000;//limit runtime
    }   

  }

}
