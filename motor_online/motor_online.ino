//declare variables for the motor pins
int motorPin1 = 3;    // Arduino pin 8 to In 1 on the ULN2003
int motorPin2 = 11;    // Arduino pin 9 to In 2 on the ULN2003
int motorPin3 = 12;   // Arduino pin 10 to In 3 on the ULN2003
int motorPin4 = 13;   // Arduino pin 11 to In 4 on the ULN2003
                      // supply power to + and - from Arduino 5vdc and gnd

int motorSpeed = 1000;  // set speed of 28BYJ-48 stepper,

//max frequency is 100hz, so max speed is 1250

//(100 steps per second 8 pulses per step so 10000 divided by 8)
// 1250 equates to apx 6 seconds per rev
int count = 0;          // count of steps made
int countsperrev = 510; // number of steps per full revolution- actually 509.5 as gear ratio is 63.684:1 apx
int revolutionsCW = 1; // number of revs required clockwise
int revolutionsCCW = 1; // number of revs required counterclockwise
int lookup[8] = {B01001, B00001, B00011, B00010, B00110,B00100,B01100, B01000};
int setTurnsCW = (revolutionsCW * countsperrev);
int setTurnsCCW = (revolutionsCCW * countsperrev);

void setup() {
  //declare the motor pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  if(count < setTurnsCW )
    anticlockwise();
  else if (count == setTurnsCW + setTurnsCCW)
    count = 0;
  else
    clockwise();
  count++;
}


void anticlockwise()
{
  for(int i = 0; i < 8; i++) // 8 pulses
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void clockwise()
{
  for(int i = 7; i >= 0; i--) // 8 pulses
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void setOutput(int out)
{
  digitalWrite(motorPin1, bitRead(lookup[out], 0));
  digitalWrite(motorPin2, bitRead(lookup[out], 1));
  digitalWrite(motorPin3, bitRead(lookup[out], 2));
  digitalWrite(motorPin4, bitRead(lookup[out], 3));
}
