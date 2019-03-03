int tempPin = A1;//Assinged pin for the LM35
double temp = 0; //Variable for the temperature


void setup() {
  Serial.begin(9600);
}

void loop() {
   temp = analogRead(tempPin);//takes the read from the pin
   temp = temp * 0.48828125;// converting the analog read to Celsius
   
   Serial.println(temp);
      
   delay(60000);                   // wait for a minute
}
