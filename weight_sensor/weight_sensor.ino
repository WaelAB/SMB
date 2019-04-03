#include "HX711.h"
HX711 scale(3, 2);// digital pins
HX711 scale2(6, 7);// digital pins

float calibration_factor = 2000; // this calibration factor is adjusted according to my load cell
float calibration_factor2 = 2500; // this calibration factor is adjusted according to my load cell
float units;// where the wight is stored
float units2;// where the wight is stored

void setup() {
  Serial.begin(9600);

  scale.set_scale();
  scale.tare();  //Reset the scale to 0
  scale2.set_scale();
  scale2.tare();  //Reset the scale to

  long zero_factor = scale.read_average(); //remove the need of tare
  long zero_factor2 = scale2.read_average(); //remove the need of tare

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

  scale2.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Reading2: ");
  units2 = scale2.get_units(), 10;
  if (units2 < 0)
  {
    units2 = 0.00;// to avoid getting minus weight values
  }

  Serial.print(units2);
  Serial.print(" grams");

  Serial.println();


  delay(2000);
}
