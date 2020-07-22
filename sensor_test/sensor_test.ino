#define IR 1
 double sensor, inches, cm ;



void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

ADMUX |=(1<<REFS0);            //<---- Setup ADC Multiplexer Selection Register, so that Vcc is the Ref, it should be "AVCC with external capacitor at AREF pin"
 ADMUX &=~(1<<REFS1);  
 ADMUX &=~(1<<ADLAR);            //<---- Setup ADC Multiplexer Selection Register, so that the result is right justified
ADCSRA |=(1<<ADEN)|  (1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);    
}

void loop() {
  // put your main code here, to run repeatedly:
 ADMUX  |=(1<<MUX0);    //<----Set Multiplexor Value to pin A0
 ADMUX  &=~(1<<MUX1);  
 ADMUX  &=~(1<<MUX2);  
 ADMUX  &=~(1<<MUX3);        
 ADCSRA |=(1<<ADSC);   
  while(ADCSRA!=ADCSRA &~(1<<ADIF)) //<---- Create a condition that waits for ADC conversion interrupt flag
  ADCSRA &=~(1<<ADIF); 
sensor= ADC;
inches= 4192.936 * pow(sensor,-0.935)-3.937;
cm= 10650.08 *pow(sensor,-0.935)-10;
delay(100);
Serial.print("centimiters   ");
Serial.println(cm);
}
