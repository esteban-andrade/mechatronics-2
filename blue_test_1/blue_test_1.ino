int Received = 0;

int a;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0 ) {
    Received = Serial.read();
  }

  if (Received == '1') {
    digitalWrite(LED_BUILTIN, HIGH);
    if (a == 0) {
      Serial.print("blue");
      a = 1;
    }


  }
  if (Received == '2') {
    digitalWrite(LED_BUILTIN, LOW);
    if (a == 1) {
      Serial.print("pink");
      a = 0;
    }



  }
   if (Received =='3'){
    if(a==0){
      Serial.print("orange");
      a=1;
      
    }
   }


}
