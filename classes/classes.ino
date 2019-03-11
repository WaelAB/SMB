#include "medicine.h"
#include "user.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"
#include "Cooler.h"

HX711 scale(3, 2);// digital pins
float calibration_factor = 2000; // this calibration factor is adjusted according to my load cell
,
boolean reply = false;

LiquidCrystal_I2C lcd(0x27, 20, 4);

medicine med[2];// create array of medicine type
User user[2]; // create array of medicine type
Cooler cool; // create object of cooler type

String uName ; // User Name
int uID; // User ID
String MedName; // Medicine Name
String Dosage; // Dosage amount
int DosageNum = 0 ; // The number of doses
boolean InStock; // status for medicine
int DTime = 0  ; // Dosage Time
int ConNum = 0 ; // Container Number
char c ; // Temporary varible to store chars
int strLen = 0; // Temporary varible to store string length

float firstWeight = 0.0;
float secondWeight = 0.0;

void setup() {

  Serial.begin(9600);           // start serial for debug
  lcd.init(); // Start the LCD
  lcd.backlight(); // Trun the light of the screen
  Wire.begin(8);                // join i2c bus with address 8 to the esp
  Wire.onReceive(receiveEvent); // register receive event from esp
  Wire.onRequest(confirm); // send confirmation to esp
  pinMode(cool.relayPin, OUTPUT);
  digitalWrite(cool.relayPin, LOW);

  SettingUp();

}

void loop() {

  cool.CheckTemp();
  receiveEvent();
  // here we will write wieght sensor code
  // check the time and if the the current time match the doses time go to next statment
  // turn on the led lights
  // start the piso 
  //print on the LCD
  // weight = scale.get_units(), 10;
  // units = scale.get_units(), 10;
  //while (true){
  // if (units - current <.10){
  // do nothing because the user hasn't took the medicine yet
  //}
   //  units= scale.get_units(), 10;
   // if (weight - units >30 && unit >0.10){// its mean that the diffirence between the old weight is more than 30 mg and the weight senor is not eampty
   //break;
   //}
  //}
  //}
  // update the state and send it to esp 

}

// function that executes whenever data is received from master

void receiveEvent() {


  while (0 < Wire.available()) {


    uID = Wire.read(); // Receive and store the user ID from the esp

    user[uID].setID(uID);
    Serial.print("User ID: ");
    Serial.println(user[uID].getID());
    //*******************

    strLen = Wire.read();
    for (int i = 0; i < strLen ; i++) {
      c = Wire.read();
      uName += c;
    }
    user[uID].setUname(uName);
    Serial.println(user[uID].getUname());
    uName = "";
    //*******************

    ConNum = Wire.read();
    med[ConNum].setContainerNum(ConNum);
    Serial.println(med[ConNum].getContainerNum());
    //*******************

    strLen = Wire.read();
    for (int i = 0; i < strLen ; i++) {
      c = Wire.read(); // Receive and store the Medicine name value char by char
      MedName += c;   // And store it into the Dosage varriable
    }

    med[ConNum].setMedName(MedName);
    Serial.println("MedName: " + med[ConNum].getMedName());
    MedName = "";
    //*******************

    strLen = Wire.read();
    for (int i = 0; i < strLen ; i++) {
      c = Wire.read(); // Receive and store the Dosage amount value char by char
      Dosage += c;    // And store it into the Dosage varriable
    }
    med[ConNum].setDosage(Dosage);
    Serial.println(med[ConNum].getDosage());
    Dosage = "";
    //*******************

    DosageNum = Wire.read(); // Receive and store the number of doses value from the esp
    Serial.println(DosageNum);
    for (int i = 0 ; i < DosageNum ; i++) {
      DTime = Wire.read(); // Receive and store the Dosage time value from the esp
      med[ConNum].setTimes(DTime, i);
      Serial.print("Time: ");
      Serial.println(med[ConNum].getTimes(i));
    }
    //*******************

    InStock = Wire.read(); // Read the In stock value from the esp
    med[ConNum].setStockState(InStock);
    Serial.print("In Stock ? ");
    Serial.println(med[ConNum].getStockState());

   confirm();

    Serial.println();            //  to newline
    user[uID].setMed(med[ConNum]);
  }
}

void confirm() {// this function will send confirmation to esp
  int GOTIT = 1;// for getting the data
  Wire.write(GOTIT);
}

void SettingUp() {
  scale.set_scale(); //
  scale.tare();  //Reset the scale to 0
  long zero_factor = scale.read_average(); // to remove the need of tare scale

  receiveEvent();
  lcd.setCursor(0, 0);// First digit , First line
  lcd.print("Hello Wael");
  // lcd.println(user[uID].getUname());
  delay(2000);

  lcd.clear();// Clear the screen from everything
  lcd.setCursor(0, 0);
  lcd.print("Please add");
  lcd.setCursor(0, 1);
  lcd.print(" a medicine");
  delay(3000);
  lcd.clear();
  
    scale.set_scale(calibration_factor); //Adjust to this calibration factor

    units = scale.get_units(), 10;
    while (units < 0.10) {// Wait untill the user to put his medicine

      units = scale.get_units(), 10;
      if (units < 0)
      {
        units = 0.00;// to avoid getting minus weight values
      }
    }

    firstWeight = units;
    Serial.print(firstWeight);
    Serial.print(" grams");
  


}

// Will use this later
// function that executes whenever data is requested from master
/*void requestEvent() {
  int i  = 9;
  Wire.write(i);  /*send string on request
  }*/
