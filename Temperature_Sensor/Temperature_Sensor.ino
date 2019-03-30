int tempPin = A1;//Assinged pin for the LM35
double temp = 0; //Variable for the temperature
int relayPin = 10;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin , OUTPUT);
  digitalWrite(relayPin, LOW);// Realy pin that sould be low( powered off) by default
}

void loop() {
  temp = analogRead(tempPin); //takes the read from the analog pin
  //temp = temp * 0.48828125; // converting the analog read to Celsius
  temp = temp * 0.32226562;
  Serial.println(temp);
  if (temp > 24) { // if the temperature is more than 24
    digitalWrite(relayPin, LOW); // turn the cooler on
  } else if (temp < 18) { // if the temprature is less than 18
    digitalWrite(relayPin, HIGH);// Turn the cooler off
  }

  delay(1000);                   // wait for a minute
}
