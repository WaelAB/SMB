#include "medicine.h" // medicine class
#include "user.h" // user class
#include <Wire.h> // i2c between arduino and esp
#include <LiquidCrystal_I2C.h> // LCD 
#include "HX711.h" // load cell and aplifire
#include "Cooler.h"// Cooler class
#include "RTClib.h" // include the clock libarary

HX711 scale(3, 2);// digital pins = DT ,SCK
//DS1302 rtc(5, 6, 7);// digital pins =RST, DAT, CLK
RTC_DS1307 rtc;
int buzzer = 4; // digital pin of the buzzer
float calibration_factor = 2000; // this calibration factor is adjusted according to my load cell
float units; // where the weight is stored


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
int ConNum = 0 ; // Container Number
char c ; // Temporary varible to store chars
int strLen = 0; // Temporary varible to store string length

String HourStr;
int HourInt = 0;
int AlarmHr = 2;

float firstWeight = 0.0;
float CurrentWeight = 0.0;
float oldWeight = 0.0;
float outOfStock = 0.0;

int GoodMorning = 1 ;// Message to esp initated with -1
int response = -1 ; //response from the esp

void setup() {

  Serial.begin(9600);           // start serial for debug

  lcd.init(); // Start the LCD
  lcd.backlight(); // Trun the light of the screen
  Wire.begin(8); // join i2c bus with address 8 to the esp

  //Wire.onRequest(HandshakeSND); // send confirmation to esp
  //Wire.onReceive(HandshakeRCV); // register receive event from esp
  Wire.onReceive(receiveEvent); // register receive event from esp
  Wire.onRequest(confirm); // send confirmation to esp

  pinMode(cool.relayPin, OUTPUT);// Realy pin
  pinMode (buzzer, OUTPUT) ; // the buzzer pin should output the buzzer sound
  digitalWrite(cool.relayPin, LOW);// Realy pin that sould be low( powered off) by default

  /*
    while (response < 0) {// to make sure both the arduino and esp are powered on so no error ouccors
      HandshakeSND();
      HandshakeRCV();
    }
  */


  if (! rtc.begin()) {// Error  handling
    Serial.println("Couldn't find RTC");
    while (1);
  }

  SettingUp();
}

void loop() {
  
  DateTime now = rtc.now(); // Creating object of DateTime
  cool.CheckTemp();
  receiveEvent();
  
  HourStr = String(now.hour(), DEC); // Getting Hours is save into String
  HourInt = HourStr.toInt(); // convert string time into integer

  if (HourInt == AlarmHr ) {// 9 is Just to test the condition
    Serial.println("ENTERED at 9");
    lcd.clear();
    lcd.println("Your medicine");
    lcd.setCursor(0, 1);
    lcd.print("time has come");

    oldWeight = scale.get_units(), 10;// weight before the alarm
    CurrentWeight = scale.get_units(), 10;// weight after the user pick his medicine

    while (true) {
      AlarmTone();

      CurrentWeight = scale.get_units(), 10;// read the weight
      Serial.print ("Current");
      Serial.println(CurrentWeight);
      delay (1000);

      if (oldWeight - CurrentWeight > 0.30 && CurrentWeight > 0.20 ) { // .20 is threshold if we use a medicne packet
        Serial.println(" YOU HAVE TOOK YOUR MEDICINE !");              // .30 treshold for the diffrerance betweem each pill
        break;
        // update status on Firebase
        //
      }
    }
    lcd.clear();
  }
  // }

  //timeTesting++;// it's just temporary

  // now we will check if the medicine is out of stock
  outOfStock = scale.get_units(), 10;
  if (outOfStock < 0.20) {
    Serial.println("Your medicine is out stock");
    //print to lcd and update to the firebase
  }
}
//------------------------------------------------------------

void receiveEvent() {
  while (0 < Wire.available()) {

    uID = Wire.read(); // Receive and store the user ID from the esp
    Serial.print("User ID: ");
    Serial.println(user[uID].getID());
    //*******************

    strLen = Wire.read();
    for (int i = 0; i < strLen ; i++) {
      c = Wire.read();
      uName += c;
    }
    if (uName != NULL) {//This condition to make sure not reciving empty data from the esp(avoid Null pointer exception)
      user[uID].setID(uID);
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
      med[ConNum].setDosesNum(DosageNum);
      for (int i = 0 ; i < DosageNum ; i++) {
//        DTime = Wire.read(); // Receive and store the Dosage time value from the esp
 //       med[ConNum].setTimes(DTime, i);
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
}
//------------------------------------------------------------

void confirm() {// this function will send confirmation to esp
  int GOTIT = 1;// for getting the data
  Wire.write(GOTIT);
}
//------------------------------------------------------------

void SettingUp() {
  scale.set_scale(); //
  scale.tare();  //Reset the scale to 0
  long zero_factor = scale.read_average(); // to remove the need of tare scale

  receiveEvent();

  lcd.setCursor(0, 0);// First digit , First line
  lcd.print("SmartMedicalBox");
  lcd.setCursor(0, 1);// First digit , Second line
  lcd.print("****Welcome****");
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
    delay(1000);
    if (units < 0)
    {
      units = 0.00;// to avoid getting minus weight values
    }
  }
  firstWeight = units;
  Serial.print(firstWeight);
  Serial.print(" grams");
}
//------------------------------------------------------------

void AlarmTone() { //this method will activate the buzzer and play a tone
  tone(buzzer, 1000);//Play a tone for pin 4 1000 Hz
  delay(500);
  tone(buzzer, 1500);//Play a tone for pin 4 1500 Hz
  delay(500);
  tone(buzzer, 2000);//Play a tone for pin 4 2000 Hz
  delay(500);
  noTone(buzzer);//Stop the tone
}
//------------------------------------------------------------

void HandshakeSND() {// this function will send int to esp
  int GoodMorning = 1;// to let it know that the arduino is powered on
  Wire.write(GoodMorning);
  Serial.println(GoodMorning);
}

void HandshakeRCV() {// this function will Recive int from esp
  int response = 1;// to  know that the esp is powered on
  response = Wire.read();
  Serial.println(response);
}
