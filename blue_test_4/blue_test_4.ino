int Received = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 
  
}

void loop() {
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
   
}
}
