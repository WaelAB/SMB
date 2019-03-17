#include "medicine.h" // medicine class
#include "user.h" // user class
#include <Wire.h> // i2c between arduino and esp
#include <LiquidCrystal_I2C.h> // LCD 
#include "HX711.h" // load cell and aplifire
#include "Cooler.h"// Cooler class
#include "RTClib.h" // include the clock libarary
//--------------
HX711 scale(3, 2);// digital pins = DT ,SCK
//--------------
RTC_DS1307 rtc;//create object from RTC module
LiquidCrystal_I2C lcd(0x27, 16, 2);//create object of the LCD
//--------------
int buzzer = 4; // digital pin of the buzzer
float calibration_factor = 2000; // this calibration factor is adjusted according to my load cell
float units; // where the weight is stored
//--------------
medicine med[2];// create array of medicine type
User user[2]; // create array of medicine type
Cooler cool; // create object of cooler type
//--------------
String uName ; // User Name
int uID; // User ID
String MedName; // Medicine Name
String Dosage; // Dosage amount
int DosageNum = 0 ; // The number of doses
boolean InStock; // status for medicine
int ConNum = 0 ; // Container Number
char c ; // Temporary varible to store chars
int strLen = 0; // Temporary varible to store string length
int DTime = 0; // Temporary to store dosage time and move it into time array
//--------------
String HourStr;
int HourInt = 0;
String MinStr ;
int MinInt = 0 ;
String SecStr;
int SecInt = 0;
int tempMin = 0;
//--------------
float firstWeight = 0.0;
float CurrentWeight = 0.0;
float oldWeight = 0.0;
float outOfStock = 0.0;
//--------------
int GotIt = -1 ;// Message to esp initated with -1
//--------------
void setup() {

  Serial.begin(9600);// start serial for debug
  Wire.begin(8); // join i2c bus with address 8 to the esp
  //--------------
  lcd.init(); // Start the LCD
  lcd.backlight(); // Trun the light of the screen
  //--------------
  Wire.onReceive(receiveEvent); // register receive event from esp
  //--------------
  pinMode(cool.relayPin, OUTPUT);// Realy pin
  pinMode (buzzer, OUTPUT) ; // the buzzer pin should output the buzzer sound
  digitalWrite(cool.relayPin, LOW);// Realy pin that sould be low( powered off) by default
  //--------------
  if (! rtc.begin()) {// Error  handling
    Serial.println("Couldn't find RTC");
    while (1);
  }

  SettingUp();
}


void loop() {

  DateTime now = rtc.now(); // Creating object of DateTime
  cool.CheckTemp();

  HourStr = String(now.hour(), DEC); // Getting Hours and save it into String
  HourInt = HourStr.toInt(); // convert string hours into integer
  MinStr =  String(now.minute(), DEC); // Getting Minutes is save into String
  MinInt = MinStr.toInt();// convert string Minute into integer
  SecStr = String (now.second(), DEC);// Getting Seconds and save it into String
  SecInt = SecStr.toInt();// convert string Seconds into integer

  //Printing time on lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  delay(1000);
  lcd.clear();

  //Serial.println(user[uID].getUname());
  if (med[0].getStockState() == true) {
    for ( int i = 0 ; i < med[0].getDosesNum(); i++) {

      if (HourInt == med[0].getTimes(i) && MinInt == 51 && SecInt <= 10) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your medicine");
        lcd.setCursor(0, 1);
        lcd.print("time has come");

        oldWeight = scale.get_units(), 10;// weight before the alarm
        CurrentWeight = scale.get_units(), 10;// weight after the user pick his medicine

        tempMin = MinInt;
        Serial.print("Temp");
        Serial.println(tempMin);
        while (true) {
          AlarmTone();

          CurrentWeight = scale.get_units(), 10;// read the weight
          CurrentWeight = abs(CurrentWeight);
          Serial.print ("Current");
          Serial.println(CurrentWeight);
          delay (1000);

          if (oldWeight - CurrentWeight > 0.30 ) { // (&& CurrentWeight > 0.20 ).20 is threshold if we use a medicne packet
            Serial.println(" YOU HAVE TOOK YOUR MEDICINE !");  // .30 treshold for the diffrerance betweem each pill

            // update status on Firebase
            // Wire.begin(10);
            //Serial.println(HourInt);
            //Wire.write(HourInt);
            // logSnd(HourInt);
            break;
          }

          MinStr =  String(now.minute(), DEC); // Getting Minutes is save into String
          MinInt = MinStr.toInt();
          Serial.println (MinInt);
          if (MinInt  - tempMin > 0) {
            Serial.println (" Time passed and med has not taken");
            // update on FB
            break;
          }
        }
      }
    }
  }
  lcd.clear();

  // now we will check if the medicine is out of stock
  outOfStock = scale.get_units(), 10;
  if (outOfStock < 0.20) {
    med[0].setStockState(false);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your medicine");
    lcd.setCursor(0, 1);
    lcd.print("is out stock");
    delay(1500);
    lcd.clear();
    //print to lcd and update to the firebase
  } else {
    med[0].setStockState(true);
  }
}
//------------------------------------------------------------

void receiveEvent() {

  while (0 < Wire.available()) {
    Serial.println("recive");

    uID = Wire.read(); // Receive and store the user ID from the esp

    strLen = Wire.read();
    for (int i = 0; i < strLen ; i++) {
      c = Wire.read();
      uName += c;
    }
    if (uName != NULL) {// avoid setting null values
      user[uID].setID(uID);
      Serial.print("User ID: ");
      Serial.println(user[uID].getID());

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



      Serial.println();            //  to newline
      GotIt = 1; //Set the flag to 1 to confirm that the data is delivered
      Serial.print("Got it");
      Serial.println(GotIt);
      user[uID].setMed(med[ConNum]);
    }
  }
}
//------------------------------------------------------------

void SettingUp() {

  lcd.setCursor(0, 0);// First digit , First line
  lcd.print("Getting Data");
  lcd.setCursor(0, 1);// First digit , Second line
  lcd.print("Please wait ...");

  while (GotIt != 1) {// the program wil wait untill the flag is set to 1 in order to procede
    Serial.println();
  }

  lcd.clear();// Clear the screen from everything
  scale.set_scale(); //
  scale.tare();  //Reset the scale to 0
  long zero_factor = scale.read_average(); // to remove the need of tare scale

  lcd.setCursor(0, 0);// First digit , First line
  lcd.print("SmartMedicalBox");
  lcd.setCursor(0, 1);// First digit , Second line
  lcd.print("****Welcome****");
  delay(2000);

  lcd.clear();// Clear the screen from everything
  lcd.setCursor(0, 0);
  lcd.print("Hello ");
  lcd.setCursor(0, 1);
  lcd.print(user[uID].getUname());
  delay(2000);

  lcd.clear();// Clear the screen from everything
  lcd.setCursor(0, 0);
  lcd.print("Please add");
  lcd.setCursor(0, 1);
  lcd.print(" a medicine");

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

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Medicine added");
  lcd.setCursor(0, 1);
  firstWeight = units;
  lcd.print(firstWeight);
  Serial.print(firstWeight);
  Serial.print(" grams");
  delay(2000);
  lcd.clear();
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
