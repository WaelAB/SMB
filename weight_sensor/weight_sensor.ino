#include "HX711.h"
HX711 scale(3, 2);// digital pins


float calibration_factor = 2000; // this calibration factor is adjusted according to my load cell
float units;// where the wight is stored


void setup() {
  Serial.begin(9600);

  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //remove the need of tare
  
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Reading: ");
  units = scale.get_units(), 10;
  if (units < 0)
  {
    units = 0.00;// to avoid getting minus weight values
  }
  
  Serial.print(units);
  Serial.print(" grams"); 
  
  Serial.println();

  
  delay(2000);
}
