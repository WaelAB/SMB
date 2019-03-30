
class Cooler {
public:
    int relayPin = 8; // Digital pin 8 for the relay
    int tempPin = A1; // Analog pin A1 for the temperature sensor
    double temp = 0; // initilaze the temp degree

    // the setup function runs once when you press reset or power the board



  public:
    void CheckTemp () {
      temp = analogRead(tempPin); //takes the read from the analog pin
     // temp = temp * 0.48828125; // converting the analog read to Celsius
     temp = temp * 0.32226562;
      //Serial.println(temp);
      if (temp > 24) { // if the temperature is more than 24
        digitalWrite(relayPin, LOW); // turn the cooler on
      } else if (temp < 18){ // if the temprature is less than 18
        digitalWrite(relayPin, HIGH);// Turn the cooler off
      }
    }
};
